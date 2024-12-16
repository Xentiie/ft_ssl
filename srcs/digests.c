/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digests.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 21:47:43 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/30 20:33:27 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _FT_FILE_DEF
#include "ft_ssl.h"
#include "inputs/inputs.h"
#include "libft/strings.h"
#include "libft/io.h"

#include <stdlib.h>

bool g_print_src;
bool g_quiet;
bool g_reverse;

static const t_long_opt longopts[] = {
	GLOBAL_LONGOPTS,
	{"print", no_argument, NULL, 'p'},
	{"quiet", no_argument, NULL, 'q'},
	{"reverse", no_argument, NULL, 'r'},
	{"string", required_argument, NULL, 's'},
	{0}};

S32 run_digest()
{
#define DEFAULT_BUFFER_SIZE 8192
	S32 ret;
	bool read_stdin;
	U64 buffer_size;
	struct s_input_source *sources;
	struct s_input_source *sources_front;
	struct s_input_source *s;

	sources = NULL;
	sources_front = NULL;

	{ /* options */
		S32 opt;

		g_print_src = FALSE;
		g_quiet = FALSE;
		g_reverse = FALSE;
		buffer_size = DEFAULT_BUFFER_SIZE;

		while ((opt = ft_getopt_long(ft_argc, (const_string *)ft_argv, GLOBAL_OPTS "pqrs:", longopts, NULL)) != -1)
		{
			switch (opt)
			{
			case 'p':
				g_print_src = TRUE;
				break;

			case 'q':
				g_quiet = TRUE;
				break;

			case 'r':
				g_reverse = TRUE;
				break;

			case 's':
				if (UNLIKELY((s = add_string_src(&sources, &sources_front, ft_argv[ft_optind])) == NULL))
					return 1;
				s->is_file = FALSE;
				s->str.data = ft_optarg;
				s->str.len = ft_strlen(ft_optarg);
				break;

			default:
				if ((ret = parse_global_opts(opt)) < 0)
					return 0;
				else if (ret > 0)
					return ret;
				break;
			case '?':
				print_help();
				return 1;
			}
		}

		read_stdin = (ft_optind >= ft_argc && sources == NULL) || g_print_src;
		for (; ft_optind < ft_argc; ft_optind++)
		{
			if (!ft_strcmp(ft_argv[ft_optind], "-"))
			{
				read_stdin = TRUE;
				continue;
			}
			if (UNLIKELY(add_file_src(&sources, &sources_front, ft_argv[ft_optind]) == NULL))
				return 1;
		}

		if (read_stdin)
		{
			if (UNLIKELY(add_stdin_src(&sources, &sources_front) == NULL))
				return 1;
		}
	}

	ret = g_func->f(sources);

	while (sources)
	{
		s = sources->next;
		free(sources);
		sources = s;
	}

	return ret;
}

static void print_digest(U8 *dgst, U64 dgst_len)
{
	for (U64 i = 0; i < dgst_len; i++)
		ft_printf("%02x", dgst[i]);
}

void print_digest_result(struct s_input_source *src, U8 *dgst, U64 dgst_len)
{
	if (src->is_file && !src->file.filename)
	{
		if (!g_quiet)
		{
			if (g_print_src)
			{
				ft_printf("(\"");
				print_escape(src->rd_buf, src->rd_buf_len);
				ft_printf("\") = ");
			}
			else
				ft_printf("(stdin) = ");
		}
		else if (g_print_src)
		{
			print_escape(src->rd_buf, src->rd_buf_len);
			ft_printf(" ");
		}
		print_digest(dgst, dgst_len);
	}
	else if (src->is_file)
	{
		if (g_reverse)
		{
			print_digest(dgst, dgst_len);
			if (!g_quiet)
				ft_printf(" %s", src->file.filename);
		}
		else
		{
			if (!g_quiet)
				ft_printf("%s (%s) = ", g_func->name, src->file.filename);
			print_digest(dgst, dgst_len);
		}
	}
	else
	{
		if (g_reverse)
		{
			print_digest(dgst, dgst_len);
			if (!g_quiet)
			{
				ft_printf(" \"");
				print_escape(src->rd_buf, src->rd_buf_len);
				ft_printf("\"");
			}
		}
		else
		{
			if (!g_quiet)
			{

				ft_printf("%s (\"", g_func->name);
				print_escape(src->rd_buf, src->rd_buf_len);
				ft_printf("\") = ");
			}
			print_digest(dgst, dgst_len);
		}
	}
	ft_printf("\n");
}

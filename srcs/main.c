/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:55:39 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/20 11:04:54 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include "libft/strings.h"
#include "libft/getopt.h"
#include "libft/io.h"

#include <stdlib.h>

struct s_func
{
	const_string name;
	// clang-format off
	S32 (*f)(void);
	// clang-format on
};

const struct s_func *g_func;
bool g_print_src;
bool g_quiet;
bool g_reverse;
U64 g_buffer_size;
U8 *g_buffer;
struct s_hash_src *g_sources;
struct s_hash_src *g_src_last;

static void print_help();

#define FUNC(f) {#f, f}
const struct s_func call_table[] = {
	FUNC(md5),
	FUNC(sha1),
	FUNC(sha224),
	FUNC(sha256),
	FUNC(whirlpool),
};
#undef FUNC

const t_long_opt longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"print", no_argument, NULL, 'p'},
	{"quiet", no_argument, NULL, 'q'},
	{"reverse", no_argument, NULL, 'r'},
	{"string", required_argument, NULL, 's'},
	{"size", required_argument, NULL, 'S'},
	{0}};

static struct s_hash_src *add_src()
{
	struct s_hash_src *new;

	if (UNLIKELY((new = malloc(sizeof(struct s_hash_src))) == NULL))
	{
		ft_errno = FT_EOMEM;
		return NULL;
	}
	ft_bzero(new, sizeof(struct s_hash_src));

	if (g_src_last == NULL)
	{
		g_src_last = new;
		g_sources = new;
	}
	else
	{
		g_src_last->next = new;
		g_src_last = new;
	}
	return new;
}

static struct s_hash_src *add_src_front()
{
	struct s_hash_src *new;

	if (UNLIKELY((new = malloc(sizeof(struct s_hash_src))) == NULL))
	{
		ft_errno = FT_EOMEM;
		return NULL;
	}
	ft_bzero(new, sizeof(struct s_hash_src));

	if (g_src_last == NULL)
	{
		g_src_last = new;
		g_sources = new;
	}
	else
	{
		new->next = g_sources;
		g_sources = new;
	}
	return new;
}

S32 main()
{
	struct s_hash_src *s;
	bool read_stdin;

	{ /* options */
		S32 opt;

		g_sources = NULL;
		g_src_last = NULL;
		g_print_src = FALSE;
		g_quiet = FALSE;
		g_reverse = FALSE;
		g_buffer = NULL;
		g_buffer_size = 8192;

		while ((opt = ft_getopt_long(ft_argc, (const_string *)ft_argv, "hpqrs:S:", longopts, NULL)) != -1)
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
				if (UNLIKELY((s = add_src()) == NULL))
					return 1;
				s->is_file = FALSE;
				s->str.data = ft_optarg;
				s->str.len = ft_strlen(ft_optarg);
				break;

			case 'S':
				if (!ft_str_isnbr(ft_optarg))
				{
					ft_fprintf(ft_fstderr, "%s: invalid argument: '%s'\n", ft_argv[0], ft_argv[ft_optind]);
					return 1;
				}
				S64 val = ft_atoi(ft_optarg);
				if (val < 0)
				{
					ft_fprintf(ft_fstderr, "%s: invalid argument: '%s'\n", ft_argv[0], ft_argv[ft_optind]);
					return 1;
				}
				g_buffer_size = val;
				break;

			case 'h':
				print_help();
				return 0;

			case '?':
			default:
				print_help();
				return 1;
			}
		}

		if (UNLIKELY((g_buffer = malloc(sizeof(U8) * g_buffer_size)) == NULL))
		{
			ft_fprintf(ft_fstderr, "%s: out of memory\n", ft_argv[0]);
			ft_errno = FT_EOMEM;
			return 1;
		}

		if ((ft_argc - ft_optind) < 1)
		{
			ft_fprintf(ft_fstderr, "%s: no function specified\n", ft_argv[0]);
			print_help();
			return 1;
		}

		g_func = NULL;
		for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
		{
			if (!ft_strcmp(ft_argv[ft_optind], call_table[i].name))
			{
				g_func = &call_table[i];
				break;
			}
		}
		if (!g_func)
		{
			ft_fprintf(ft_fstderr, "%s: %s: invalid function\n", ft_argv[0], ft_argv[1]);
			return 1;
		}
		ft_optind++;

		read_stdin = (ft_optind >= ft_argc && g_sources == NULL) || g_print_src;
		for (; ft_optind < ft_argc; ft_optind++)
		{
			if (!ft_strcmp(ft_argv[ft_optind], "-"))
			{
				read_stdin = TRUE;
				continue;
			}

			if (UNLIKELY((s = add_src()) == NULL))
				return 1;
			s->is_file = TRUE;
			s->file.filename = ft_argv[ft_optind];
			s->file.f = ft_fopen(s->file.filename, "r");
			if (s->file.f == NULL)
			{
				ft_fprintf(ft_fstderr, "%s: couldn't open '%s'\n", ft_argv[0], s->file.filename);
				continue;
			}
			s->rd_buf_len = 0;
		}

		if (read_stdin)
		{

			if (UNLIKELY((s = add_src_front()) == NULL))
				return 1;
			s->is_file = TRUE;
			s->file.filename = NULL;
			s->file.f = ft_fstdin;
		}
	}
	return g_func->f();
}

void __attribute__((destructor)) destructor()
{
	struct s_hash_src *s;

	free(g_buffer);
	if (ft_errno != 0)
		ft_fprintf(ft_fstderr, "%s: exiting with error: %s\n", ft_argv[0], ft_strerror(ft_errno));
	s = g_sources;
	while (s)
	{
		g_sources = s;
		s = s->next;
		free(g_sources);
	}
}

struct s_hash_blk read_data(struct s_hash_src *src)
{
	struct s_hash_blk blk;
	S64 ret;
	U64 cpy;

	blk = (struct s_hash_blk){NULL, 0};
	if (src->is_file && src->file.f != NULL)
	{
		if (UNLIKELY((ret = ft_fread(src->file.f, (char *)g_buffer, g_buffer_size)) == -1))
		{
			ft_fprintf(ft_fstderr, "%s: couldn't read file '%s': %s\n", ft_argv[0], src->file.filename, ft_strerror2(ft_errno));
			exit(1);
		}
		blk.ptr = g_buffer;
		blk.size = ret;

		if (ret == 0)
		{
			if (src->file.f != ft_fstdin)
				ft_fclose(src->file.f);
			src->file.f = NULL;
		}
	}
	else
	{
		blk.ptr = (const U8 *)src->str.data;
		blk.size = src->str.len;
		src->str.len = 0;
	}

	if (src->rd_buf_len < sizeof(src->rd_buf))
	{
		cpy = blk.size;
		if (cpy > sizeof(src->rd_buf) - src->rd_buf_len)
			cpy = sizeof(src->rd_buf) - src->rd_buf_len;
		ft_memcpy(src->rd_buf + src->rd_buf_len, blk.ptr, cpy);
		src->rd_buf_len += cpy;
	}
	return blk;
}

static void print_result_data(U8 *data, U64 data_len)
{
	for (U64 i = 0; i < data_len; i++)
		ft_printf("%02x", data[i]);
}

static void print_escape(const_string content, U64 len)
{
	char buf1[2] = {'\\'};
	U64 i;

	i = 0;
	while (i < len)
	{
		if (ft_isprint(content[i]))
			ft_fwrite(ft_fstdout, (char *)&content[i], 1);
		else if ((content[i] >= 0 && content[i] <= 6) || (content[i] >= 14 && content[i] <= 31) || content[i] == 127)
		{
			buf1[1] = content[i] + '0';
			ft_fwrite(ft_fstdout, buf1, 2);
		}
		else
		{
			switch (content[i])
			{
			case '\a':
				buf1[1] = 'a';
				break;
			case '\b':
				buf1[1] = 'b';
				break;
			case '\t':
				buf1[1] = 't';
				break;
			case '\n':
				buf1[1] = 'n';
				break;
			case '\v':
				buf1[1] = 'v';
				break;
			case '\f':
				buf1[1] = 'f';
				break;
			case '\r':
				buf1[1] = 'r';
				break;
			}
			ft_fwrite(ft_fstdout, buf1, 2);
		}
		i++;
	}
}

void print_result(struct s_hash_src *src, U8 *data, U64 data_len)
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
		print_result_data(data, data_len);
	}
	else if (src->is_file)
	{
		if (g_reverse)
		{
			print_result_data(data, data_len);
			if (!g_quiet)
				ft_printf(" %s", src->file.filename);
		}
		else
		{
			if (!g_quiet)
				ft_printf("%s (%s) = ", g_func->name, src->file.filename);
			print_result_data(data, data_len);
		}
	}
	else
	{
		if (g_reverse)
		{
			print_result_data(data, data_len);
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
			print_result_data(data, data_len);
		}
	}
	ft_printf("\n");
}

static void print_help()
{
	ft_printf(
		"Usage\n\
  %s <function> [options]\n\
\n\
Functions:\n",
		ft_argv[0]);

	for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
		ft_printf("  %s\n", call_table[i].name);

	ft_printf("\n\
Options:\n\
  -h --help          print help and exit`\n\
  -q --quiet         quiet output\n\
  -p --print         print source in output\n\
  -r --reverse       reverse filename/hash output orger\n\
  -s --string <str>  computes the hash for the specified string\n\
  -S --size <val>    sets the file read buffer size\n");
}

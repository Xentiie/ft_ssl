/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:55:39 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/13 15:44:29 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include "libft/std.h"
#include "libft/strings.h"
#include "libft/getopt.h"
#include "libft/io.h"
#include "libft/lists.h"

#include <stdlib.h>

/*
open
close
read
write
malloc
free
*/

struct s_func
{
	const_string name;
	S32(*f)
	(struct s_hash_src *);
};

const struct s_func *func;
bool print_src;
bool quiet;
bool reverse;

static void print_help();

#define FUNC(name, f) {name, f}
const struct s_func call_table[] = {
	FUNC("md5", md5)};
#undef FUNC

const t_long_opt longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"print", no_argument, NULL, 'p'},
	{"quiet", no_argument, NULL, 'q'},
	{"reverse", no_argument, NULL, 'r'},
	{"string", required_argument, NULL, 's'},
	{0}};

bool add_src(struct s_hash_src **sources, struct s_hash_src src)
{
	struct s_hash_src *new;

	if (UNLIKELY((new = ft_memdup(&src, sizeof(struct s_hash_src))) == NULL))
	{
		ft_errno = FT_EOMEM;
		return FALSE;
	}

	new->next = *sources;
	*sources = new;
	return TRUE;
}

S32 main()
{
	struct s_hash_src *sources;
	struct s_hash_src *s;

	U8 *ptr;
	bool read_stdin;
	U64 len;

	sources = NULL;
	{ /* options */
		S32 opt;

		print_src = FALSE;
		quiet = FALSE;
		reverse = FALSE;

		while ((opt = ft_getopt_long(ft_argc, (const_string *)ft_argv, "hpqrs:", longopts, NULL)) != -1)
		{
			switch (opt)
			{
			case 'p':
				print_src = TRUE;
				break;

			case 'q':
				quiet = TRUE;
				break;

			case 'r':
				reverse = TRUE;
				break;

			case 's':
				if (UNLIKELY((ptr = (U8 *)ft_strdup_l(ft_optarg, &len)) == NULL))
					goto exit_err;

				if (UNLIKELY(!add_src(&sources, (struct s_hash_src){
													.filename = NULL,
													.content = ptr,
													.content_len = len,
													.stdin = FALSE})))
				{
					free(ptr);
					goto exit_err;
				}
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

		if ((ft_argc - ft_optind) < 1)
		{
			ft_fprintf(ft_fstderr, "%s: no function specified\n", ft_argv[0]);
			print_help();
			return 1;
		}

		func = NULL;
		for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
		{
			if (!ft_strcmp(ft_argv[ft_optind], call_table[i].name))
			{
				func = &call_table[i];
				break;
			}
		}
		if (!func)
		{
			ft_fprintf(ft_fstderr, "%s: %s: invalid function\n", ft_argv[0], ft_argv[1]);
			goto exit_err;
		}
		ft_optind++;

		read_stdin = (ft_optind >= ft_argc && sources == NULL) || print_src;
		while (ft_optind < ft_argc)
		{
			t_file *f;

			if (!ft_strcmp(ft_argv[ft_optind], "-"))
			{
				read_stdin = TRUE;
				continue;
			}

			if ((f = ft_fopen(ft_argv[ft_optind], "r")) == NULL)
			{
				ft_fprintf(ft_fstderr, "%s: couldn't open '%s': %s\n", ft_argv[0], ft_argv[ft_optind], ft_strerror2(ft_errno));
				continue;
			}

			if (UNLIKELY((ptr = ft_freadfile(f, &len)) == NULL))
			{
				ft_fclose(f);
				ft_fprintf(ft_fstderr, "%s: couldn't read '%s': %s\n", ft_argv[0], ft_argv[ft_optind], ft_strerror2(ft_errno));
				continue;
			}
			ft_fclose(f);

			if (UNLIKELY(!add_src(&sources, (struct s_hash_src){
												.filename = ft_argv[ft_optind],
												.content = ptr,
												.content_len = len,
												.stdin = FALSE})))
			{
				free(ptr);
				goto exit_err;
			}
			ft_optind++;
		}

		if (read_stdin)
		{
			if (UNLIKELY((ptr = ft_freadfile(ft_fstdin, &len)) == NULL))
			{
				ft_fprintf(ft_fstderr, "%s: couldn't read stdin: %s\n", ft_argv[0], ft_strerror2(ft_errno));
				goto exit_err;
			}

			if (UNLIKELY(!add_src(&sources, (struct s_hash_src){
												.filename = NULL,
												.content = ptr,
												.content_len = len,
												.stdin = TRUE})))
			{
				free(ptr);
				goto exit_err;
			}
		}
	}

	return func->f(sources);

exit_err:
	ft_fprintf(ft_fstderr, "%s: exiting with error %s\n", ft_argv[0], ft_strerror(ft_errno));
	s = sources;
	while (s)
	{
		sources = s;
		s = s->next;
		free(sources);
	}
	return 1;
}

/*
Aucun arguments:
stdin: (stdin)= ...
-s: MD5 ("...")= ...
*/

static void print_result_data(U8 *data, U64 data_len)
{
	for (U64 i = 0; i < data_len; i++)
		ft_printf("%02x", data[i]);
}

static string clean_nl(const_string str, U64 len)
{
	const_string ptr;
	string cleaned;
	U32 nl_cnt;
	U64 i, j;

	nl_cnt = 0;
	ptr = str;
	while ((ptr = ft_strchr(ptr, '\n')) != NULL)
	{
		ptr++;
		nl_cnt++;
	}

	if (UNLIKELY((cleaned = malloc(sizeof(U8) * (len + (nl_cnt * 2) + 1))) == NULL))
	{
		ft_fprintf(ft_fstderr, "%s: out of memory\n", ft_argv[0]);
		exit(1);
	}
	ft_bzero(cleaned, sizeof(U8) * (len + (nl_cnt * 2) + 1));

	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] == '\n')
		{
			cleaned[j++] = '\\';
			cleaned[j++] = 'n';
		}
		else
			cleaned[j++] = str[i];
		i++;
	}
	return cleaned;
}

void print_result(struct s_hash_src *src, U8 *data, U64 data_len)
{
	string cleaned;

	if (src->stdin)
	{
		if (!quiet)
		{
			cleaned = clean_nl((const_string)src->content, src->content_len);
			if (print_src)
				ft_printf("(\"%s\") = ", cleaned);
			else
				ft_printf("(stdin) = ");
			free(cleaned);
		}
		print_result_data(data, data_len);
	}
	else if (src->filename)
	{
		if (reverse)
		{
			print_result_data(data, data_len);
			ft_printf(" %s", src->filename);
		}
		else
		{
			ft_printf("%s (%s) = ", func->name, src->filename);
			print_result_data(data, data_len);
		}
	}
	else
	{
		if (reverse)
		{
			print_result_data(data, data_len);
			cleaned = clean_nl((const_string)src->content, src->content_len);
			ft_printf(" \"%s\"", cleaned);
			free(cleaned);
		}
		else
		{
			cleaned = clean_nl((const_string)src->content, src->content_len);
			ft_printf("%s (\"%s\") = ", func->name, cleaned);
			free(cleaned);
			print_result_data(data, data_len);
		}
	}
	ft_printf("\n");
}

static void print_help()
{
	ft_printf(
		"Usage\n\
  %s <function> [flags]\n\
\n\
Functions:\n",
		ft_argv[0]);

	for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
		ft_printf("  %s\n", call_table[i].name);

	ft_printf("\n\
Flags:\n\
  -h --help          print help and exit`\n\
  -q --quiet         quiet output\n\
  -p --print         print source in output\n\
  -r --reverse       reverse filename/hash output orger\n\
  -s --string <str>  computes the hash for the specified string\n");
}
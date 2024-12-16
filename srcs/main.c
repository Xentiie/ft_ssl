/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:55:39 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 10:56:24 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include "libft/strings.h"
#include "libft/getopt.h"
#include "libft/io.h"

#include <stdlib.h>
#include <readline/readline.h>

const struct s_ssl_func *g_func;

#define FUNC_MESSAGE_DIGEST(f) {#f, f, MESSAGE_DIGEST}
#define CIPHER_MESSAGE_DIGEST(f) {#f, f, CIPHER}
const struct s_ssl_func call_table[] = {
	FUNC_MESSAGE_DIGEST(md5),
	FUNC_MESSAGE_DIGEST(sha1),
	FUNC_MESSAGE_DIGEST(sha224),
	FUNC_MESSAGE_DIGEST(sha256),
	FUNC_MESSAGE_DIGEST(whirlpool),

	CIPHER_MESSAGE_DIGEST(base64),
	{"des-ecb", des_ecb, CIPHER},
	{"des-cbc", des_cbc, CIPHER},
};

S32 main()
{
	if (ft_argc < 2)
	{
		ft_fprintf(ft_fstderr, "%s: no functions specified\n", ft_argv[0]);
		return 1;
	}

	if (!ft_strcmp(ft_argv[1], "-h") || !ft_strcmp(ft_argv[1], "--help"))
	{
		print_help();
		return 0;
	}

	g_func = NULL;
	for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
	{
		if (!ft_strcmp(ft_argv[1], call_table[i].name))
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
	ft_argv[1] = ft_argv[0];
	ft_argv = &ft_argv[1];
	ft_argc--;

	switch (g_func->type)
	{
	case MESSAGE_DIGEST:
		return run_digest();
	case CIPHER:
		return run_cipher();
	default:
		return 1000;
	}
}

S32 parse_global_opts(S32 opt)
{
#define _exit_err 1
#define _exit_noerr -1
#define _noexit 0
	switch (opt)
	{
	case 'S':
		if (!ft_str_isnbr(ft_optarg))
		{
			ft_fprintf(ft_fstderr, "%s: invalid argument: '%s'\n", ft_argv[0], ft_argv[ft_optind]);
			return _exit_err;
		}
		S64 val = ft_atoi(ft_optarg);
		if (val < 0)
		{
			ft_fprintf(ft_fstderr, "%s: invalid argument: '%s'\n", ft_argv[0], ft_argv[ft_optind]);
			return _exit_err;
		}
		set_buffer_size(val);
		break;

	case 'h':
		print_help();
		return _exit_noerr;
	case '?':
	default:
		print_help();
		return _exit_err;
	}
	return _noexit;
#undef _exit_err
#undef _exit_noerr
#undef _noexit
}

void print_escape(const U8 *content, U64 len)
{
	char buf1[2] = {'\\'};
	U64 i;

	i = 0;
	while (i < len)
	{
		if (ft_isprint(content[i]))
			ft_fwrite(ft_fstdout, (char *)&content[i], 1);
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
			default:
				buf1[1] = content[i] + '0';
				break;
			}
			ft_fwrite(ft_fstdout, buf1, 2);
		}
		i++;
	}
}

void print_help()
{
	ft_printf(
		"Usage\n\
  %s <function> [global options] [function specific options]\n\
\n",
		ft_argv[0]);
	ft_printf("Options:\n\
  -h --help          print help and exits\n\
  -S --size          sets the file read buffer size\n\n");

	ft_printf("Message digests:\n");
	for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
	{
		if (call_table[i].type == MESSAGE_DIGEST)
			ft_printf("    %s\n", call_table[i].name);
	}
	ft_printf("  Options:\n\
    -h --help          print help and exits\n\
    -q --quiet         quiet output\n\
    -p --print         print source in output\n\
    -r --reverse       reverse filename/hash output order\n");
	ft_printf("\n");

	ft_printf("Ciphers:\n");
	for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
	{
		if (call_table[i].type == CIPHER)
			ft_printf("    %s\n", call_table[i].name);
	}
	ft_printf("  Options:\n\
    -h --help          print help and exits\n\
    -q --quiet         quiet output\n\
    -p --print         print source in output\n\
    -r --reverse       reverse filename/hash output order\n");
	ft_printf("\n");
}

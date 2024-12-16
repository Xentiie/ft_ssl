/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ciphers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 06:17:41 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/30 20:34:16 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/strings.h"
#include "libft/io.h"

static const t_long_opt longopts[] = {
	GLOBAL_LONGOPTS,
	{"encode", no_argument, NULL, 'e'},
	{"decode", no_argument, NULL, 'd'},
	{0}};

enum e_cipher_mode g_cipher_mode;

S32 run_cipher()
{
	S32 ret;
	struct s_input_source *sources;
	struct s_input_source *sources_last;
//	struct s_input_source *s;
	t_file *output_file;

	{
		S32 opt;

		g_cipher_mode = ENCODE;

		while ((opt = ft_getopt_long(ft_argc, (const_string *)ft_argv, GLOBAL_OPTS "edi:o:", longopts, NULL)) != -1)
		{
			switch (opt)
			{
			case 'e':
				g_cipher_mode = ENCODE;
				break;

			case 'd':
				g_cipher_mode = DECODE;
				break;

			case 'i':
				if (UNLIKELY(add_file_src(&sources, &sources_last, (string)ft_optarg) == NULL))
				{
					ft_fprintf(ft_fstderr, "%s: couldn't open file '%s': %s\n", ft_argv[0], ft_optarg, ft_strerror2(ft_errno));
					return 1;
				}
				break;

			case 'o':
				if ((output_file = ft_fopen(ft_optarg, "w")) == NULL)
				{
					ft_fprintf(ft_fstderr, "%s: couldn't open file '%s': %s\n", ft_argv[0], ft_optarg, ft_strerror2(ft_errno));
					return 1;
				}
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

		
	}
}

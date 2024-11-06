/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:36 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/06 14:33:55 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/std.h"
#include "libft/io.h"
#include "libft/getopt.h"

S32 md5()
{
	string src_file;
	bool quiet;
	bool reverse;

	{ /* options */
		S32 opt;

		quiet = FALSE;

		while ((opt = ft_getopt(ft_argc, (const_string *)ft_argv, "pqrs")))
		{
			switch (opt)
			{
			case 'p':
				break;

			case 'q':
				quiet = TRUE;
				break;

			case 'r':
				reverse = TRUE;
				break;

			case 's':
				break;

			case '?':
			default:
				print_help();
				return 1;
			}
		}

		ft_optind++; /* skip 'md5' */
	}
}

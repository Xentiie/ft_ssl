/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:55:39 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/06 14:33:47 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include "libft/std.h"
#include "libft/strings.h"
#include "libft/getopt.h"
#include "libft/io.h"

/*
open
close
read
write
malloc
free
*/

static void print_help();

S32 main()
{
	const struct
	{
		const_string name;
		S32 (* const f)();
	} call_table[] = {
		{"md5", md5}};

	if (ft_argc < 2)
	{
		ft_fprintf(ft_fstderr, "%s: no function specified\n");
		return 1;
	}

	for (U8 i = 0; i < (sizeof(call_table) / sizeof(call_table[0])); i++)
	{
		if (!ft_strcmp(ft_argv[1], call_table[i].name))
			return call_table[i].f();
	}

	ft_fprintf(ft_fstderr, "%s: %s: invalid function\n", ft_argv[0], ft_argv[1]);
	return 1;
}

static void print_help()
{
	ft_printf(
		"Usage\n\
  %s <function>\n\
\n\
Options:\n\
  <target>                        dns name or ip address\n\
  -f (--file) <filename>          file name containing list of IPs to scan\n\
  -h (--help)                     print help and exit\n\
  -i (--ip) <IP address>          ip address to scan\n\
  -I (--interface) <Interface>    interface to use\n\
  -p (--ports) <ports>            port range to scan\n\
  -s (--speedup) <threads>        number of parallel threads to use [1;250]\n\
  -S (--scan)                     scan method to use. Can be any combination of SYN/NULL/FIN/XMAS/ACK/UDP\n\
  -t (--timeout) <timeout>        sockets timeout\n",
		ft_argv[0]);
}
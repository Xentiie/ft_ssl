/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:50 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/12 18:57:15 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
#define FT_SSL_H

#include "libft/std.h"

struct s_hash_src
{
	bool stdin;
	const_string filename;
	U8 *content;
	U64 content_len;
	struct s_hash_src *next;
};

void print_result(struct s_hash_src *src, U8 *data, U64 data_len);

S32 md5(struct s_hash_src *);

#endif

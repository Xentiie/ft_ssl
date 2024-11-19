/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:50 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/18 15:57:13 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
#define FT_SSL_H

#include "libft/std.h"

struct s_hash_blk
{
	const U8 *ptr;
	U64 size;
};

struct s_hash_src
{
	bool is_file;
	union {
		struct {
			const_string filename;
			t_file *f;
		} file;
		struct {
			const_string data;
			U64 len;
		} str;
	};
	char rd_buf[50];
	U64 rd_buf_len;
	struct s_hash_src *next;
};
extern struct s_hash_src *g_sources;

struct s_hash_blk read_data(struct s_hash_src *src);
void print_result(struct s_hash_src *src, U8 *data, U64 data_len);

S32 md5();
S32 sha1();
S32 sha256();
S32 sha224();
S32 whirlpool();

#endif

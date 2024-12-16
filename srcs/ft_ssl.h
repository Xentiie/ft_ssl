/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:50 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 10:59:48 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
#define FT_SSL_H

#include "libft/std.h"
#include "libft/getopt.h"
#include "inputs/inputs.h"

typedef S32 (*f_ssl_func)();

enum e_func_type
{
	MESSAGE_DIGEST,
	CIPHER,
};

struct s_ssl_func
{
	const_string name;
	// clang-format off
	f_ssl_func f;
	// clang-format on
	enum e_func_type type;
};

#define GLOBAL_OPTS "hS:"
#define GLOBAL_LONGOPTS {"help", no_argument, NULL, 'h'}, {"size", required_argument, NULL, 'S'}
extern const struct s_ssl_func *g_func;
S32 parse_global_opts(S32 opt);
void print_escape(const U8 *content, U64 len);
void print_help();

/* message digests */
S32 run_digest();

extern bool g_print_src;
extern bool g_quiet;
extern bool g_reverse;
void print_digest_result(struct s_input_source *src, U8 *data, U64 data_len);

S32 md5(struct s_input_source *srcs);
S32 sha1(struct s_input_source *srcs);
S32 sha224(struct s_input_source *srcs);
S32 sha256(struct s_input_source *srcs);
S32 whirlpool(struct s_input_source *srcs);

/* ciphers */
enum e_cipher_mode
{
	ENCODE,
	DECODE	
};

extern enum e_cipher_mode g_cipher_mode;

S32 run_cipher();

S32 base64();
S32 des();
S32 des_ecb();
S32 des_cbc();

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 08:34:30 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 10:49:41 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if !defined(INPUTS_H)
#define INPUTS_H

#include "libft/std.h"

struct s_input_chunk
{
	const U8 *ptr;
	U64 size;
};

struct s_input_source
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
	U8 rd_buf[70];
	U64 rd_buf_len;
	struct s_input_source *next;
};

void set_buffer_size(U64 size);

struct s_input_source *new_input_source_back(struct s_input_source **lst, struct s_input_source **last);
struct s_input_source *new_input_source_front(struct s_input_source **lst, struct s_input_source **last);

struct s_input_source *add_string_src(struct s_input_source **lst, struct s_input_source **last, string str);
struct s_input_source *add_file_src(struct s_input_source **lst, struct s_input_source **last, string path);
struct s_input_source *add_stdin_src(struct s_input_source **lst, struct s_input_source **last);

struct s_input_chunk read_chunk(struct s_input_source *src);

#endif
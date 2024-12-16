/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 08:36:46 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/30 20:32:39 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _FT_FILE_DEF
#include "inputs.h"
#include "libft/io.h"
#include "libft/strings.h"

#include <stdlib.h>

#define DEFAULT_BUFFER_SIZE 8192
static U64 g_buff_size = DEFAULT_BUFFER_SIZE;

void set_buffer_size(U64 size)
{
	g_buff_size = size;
}

struct s_input_source *new_input_source_back(struct s_input_source **lst, struct s_input_source **last)
{
	struct s_input_source *new;

	if (UNLIKELY((new = malloc(sizeof(struct s_input_source))) == NULL))
	{
		ft_errno = FT_EOMEM;
		return NULL;
	}
	ft_bzero(new, sizeof(struct s_input_source));

	if ((*last) == NULL)
	{
		(*last) = new;
		(*lst) = new;
	}
	else
	{
		(*last)->next = new;
		(*last) = new;
	}
	return new;
}

struct s_input_source *new_input_source_front(struct s_input_source **lst, struct s_input_source **last)
{
	struct s_input_source *new;

	if (UNLIKELY((new = malloc(sizeof(struct s_input_source))) == NULL))
	{
		ft_errno = FT_EOMEM;
		return NULL;
	}
	ft_bzero(new, sizeof(struct s_input_source));

	if ((*last) == NULL)
	{
		(*last) = new;
		(*lst) = new;
	}
	else
	{
		new->next = (*lst);
		(*lst) = new;
	}
	return new;
}

struct s_input_source *add_string_src(struct s_input_source **lst, struct s_input_source **last, string str)
{
	struct s_input_source *s;

	if (UNLIKELY((s = new_input_source_back(lst, last)) == NULL))
		return NULL;
	s->is_file = FALSE;
	s->str.data = str;
	s->str.len = ft_strlen(str);
	s->rd_buf_len = 0;
	return s;
}

struct s_input_source *add_file_src(struct s_input_source **lst, struct s_input_source **last, string path)
{
	struct s_input_source *s;
	t_file *f;

	if ((f = ft_fopen(path, "r")) == NULL)
		return NULL;

	if (UNLIKELY((s = new_input_source_back(lst, last)) == NULL))
		return NULL;
	s->is_file = TRUE;
	s->file.f = f;
	ft_fsetbuf(s->file.f, NULL, g_buff_size, FT_IO_FULL_BUFFERED);
	s->file.filename = path;
	s->rd_buf_len = 0;
	return s;
}

struct s_input_source *add_stdin_src(struct s_input_source **lst, struct s_input_source **last)
{
	struct s_input_source *s;

	if (UNLIKELY((s = new_input_source_front(lst, last)) == NULL))
		return NULL;
	s->is_file = TRUE;
	s->file.f = ft_fstdin;
	ft_fsetbuf(s->file.f, NULL, g_buff_size, FT_IO_FULL_BUFFERED);
	s->file.filename = NULL;
	s->rd_buf_len = 0;
	return s;
}

struct s_input_chunk read_chunk(struct s_input_source *src)
{
	struct s_input_chunk blk;
	S64 ret;
	U64 cpy;

	blk = (struct s_input_chunk){NULL, 0};
	if (src->is_file && src->file.f != NULL)
	{
		if (UNLIKELY((ret = ft_fread(src->file.f, (char *)src->file.f->buff, src->file.f->buff_size)) == -1))
		{
			ft_fprintf(ft_fstderr, "%s: couldn't read file '%s': %s\n", ft_argv[0], src->file.filename, ft_strerror2(ft_errno));
			exit(1);
		}
		blk.ptr = src->file.f->buff;
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

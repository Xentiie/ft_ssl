/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:36 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/12 16:15:39 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include "libft/std.h"
#include "libft/io.h"
#include "libft/getopt.h"
#include "libft/lists.h"
#include "libft/crypt/md5.h"

#include <stdlib.h>

S32 md5(struct s_hash_src *src)
{
	struct s_md5_state state;
	U8 digest[16];

	while (src)
	{
		ft_md5_init(&state);
		ft_md5_update(&state, src->content, src->content_len);
		ft_md5_final(&state, digest);

		print_result(src, digest, sizeof(digest));

		src = src->next;
	}

	return 0;
}

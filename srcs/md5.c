/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:36 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/18 04:38:35 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/md5.h"

S32 md5()
{
	struct s_hash_src *src;
	struct s_hash_blk blk;
	struct s_md5_state state;
	U8 digest[16];

	src = g_sources;
	while (src)
	{
		ft_md5_init(&state);
		while ((blk = read_data(src)).size != 0)
			ft_md5_update(&state, blk.ptr, blk.size);
		ft_md5_final(&state, digest);
		print_result(src, digest, sizeof(digest));
		src = src->next;
	}

	return 0;
}

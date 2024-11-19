/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:08:13 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/19 04:03:50 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/whirlpool.h"

S32 whirlpool()
{
	struct s_hash_src *src;
	struct s_hash_blk blk;
	struct s_whirlpool_state state;
	U8 digest[64];

	src = g_sources;
	while (src)
	{
		ft_whirlpool_init(&state);
		while ((blk = read_data(src)).size != 0)
			ft_whirlpool_update(&state, blk.ptr, blk.size);
		ft_whirlpool_final(&state, digest);
		print_result(src, digest, sizeof(digest));

		src = src->next;
	}

	return 0;
}

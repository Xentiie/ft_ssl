/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 02:16:37 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/18 05:37:20 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/sha.h"

S32 sha224()
{
	struct s_hash_src *src;
	struct s_hash_blk blk;
	struct s_sha256_state state;
	U8 digest[28];

	src = g_sources;
	while (src)
	{
		ft_sha224_init(&state);
		while ((blk = read_data(src)).size != 0)
			ft_sha224_update(&state, blk.ptr, blk.size);
		ft_sha224_final(&state, digest);
		print_result(src, digest, sizeof(digest));

		src = src->next;
	}

	return 0;
}

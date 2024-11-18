/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:02:03 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/18 04:38:17 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/sha.h"

S32 sha1()
{
	struct s_hash_src *src;
	struct s_hash_blk blk;
	struct s_sha1_state state;
	U8 digest[20];

	src = g_sources;
	while (src)
	{
		ft_sha1_init(&state);
		while ((blk = read_data(src)).size != 0)
			ft_sha1_update(&state, blk.ptr, blk.size);
		ft_sha1_final(&state, digest);
		print_result(src, digest, sizeof(digest));
		src = src->next;
	}

	return 0;
}

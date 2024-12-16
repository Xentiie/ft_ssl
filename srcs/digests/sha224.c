/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 02:16:37 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 09:20:38 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/sha.h"

S32 sha224(struct s_input_source *srcs)
{
	struct s_input_chunk chk;
	struct s_sha256_state state;
	U8 digest[28];

	while (srcs)
	{
		ft_sha224_init(&state);
		while ((chk = read_chunk(srcs)).size != 0)
			ft_sha224_update(&state, chk.ptr, chk.size);
		ft_sha224_final(&state, digest);
		print_digest_result(srcs, digest, sizeof(digest));

		srcs = srcs->next;
	}

	return 0;
}

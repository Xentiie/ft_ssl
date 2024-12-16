/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:08:13 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 09:20:38 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/whirlpool.h"

S32 whirlpool(struct s_input_source *srcs)
{
	struct s_input_chunk chk;
	struct s_whirlpool_state state;
	U8 digest[64];

	while (srcs)
	{
		ft_whirlpool_init(&state);
		while ((chk = read_chunk(srcs)).size != 0)
			ft_whirlpool_update(&state, chk.ptr, chk.size);
		ft_whirlpool_final(&state, digest);
		print_digest_result(srcs, digest, sizeof(digest));

		srcs = srcs->next;
	}

	return 0;
}

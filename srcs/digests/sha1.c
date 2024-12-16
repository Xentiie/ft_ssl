/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:02:03 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 09:20:38 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft/crypt/sha.h"

S32 sha1(struct s_input_source *srcs)
{
	struct s_input_chunk chk;
	struct s_sha1_state state;
	U8 digest[20];

	while (srcs)
	{
		ft_sha1_init(&state);
		while ((chk = read_chunk(srcs)).size != 0)
			ft_sha1_update(&state, chk.ptr, chk.size);
		ft_sha1_final(&state, digest);
		print_digest_result(srcs, digest, sizeof(digest));
		srcs = srcs->next;
	}

	return 0;
}

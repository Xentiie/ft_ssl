/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:36 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/22 09:20:38 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "inputs/inputs.h"
#include "libft/crypt/md5.h"

S32 md5(struct s_input_source *srcs)
{
	struct s_input_chunk chk;
	struct s_md5_state state;
	U8 digest[16];

	while (srcs)
	{
		ft_md5_init(&state);
		while ((chk = read_chunk(srcs)).size != 0)
			ft_md5_update(&state, chk.ptr, chk.size);
		ft_md5_final(&state, digest);
		print_digest_result(srcs, digest, sizeof(digest));
		srcs = srcs->next;
	}

	return 0;
}

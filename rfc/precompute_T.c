/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precompute_T.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:47:14 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/09 02:47:14 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/io.h"
#include <stdlib.h>
#include <math.h>

/*
   This step uses a 64-element table T[1 ... 64] constructed from the
   sine function. Let T[i] denote the i-th element of the table, which
   is equal to the integer part of 4294967296 times abs(sin(i)), where i
   is in radians. The elements of the table are given in the appendix.
*/

/* gcc ./precompute_T.c -I../../libft -L../../libft -lft_linux -lm */
void main()
{
	U32 T[64];
	ft_printf("S32 T[] = {\n\t");
	for (U8 i = 0; i < 64; i++)
	{
		ft_printf("%#x", (U32)(4294967296U * fabs(sin(i))));
		if (i != 63)
			ft_printf(", ");
		if ((i + 1) % 8 == 0)
		{
			ft_printf("\n");
			if (i != 63)
				ft_printf("\t");
		}
	}
	ft_printf("}\n");
}
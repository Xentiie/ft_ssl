/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:06:36 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/07 02:23:28 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/std.h"
#include "libft/io.h"
#include "libft/getopt.h"

unsigned char *pad_message(const unsigned char *message, size_t original_len, size_t *padded_len)
{
	// Calculate the length of the padded message
	size_t padding_len = 0;

	// 64 bits at the end will be reserved for the length of the original message
	size_t total_len = original_len + 1 + 8;
	size_t remainder = total_len % 64;

	if (remainder <= 56)
		padding_len = 56 - remainder;
	else
		padding_len = 56 + (64 - remainder);

	*padded_len = original_len + 1 + padding_len + 8;

	// Allocate memory for the padded message
	unsigned char *padded_message = (unsigned char *)malloc(*padded_len);
	if (!padded_message)
		return NULL;

	// Copy the original message to the padded message
	memcpy(padded_message, message, original_len);

	// Add the padding: a single 1 bit (0x80 in hex) followed by 0 bits
	padded_message[original_len] = 0x80;
	memset(padded_message + original_len + 1, 0, padding_len);

	// Append the original message length in bits as a 64-bit big-endian integer
	U64 original_len_bits = original_len * 8;
	for (int i = 0; i < 8; i++)
	{
		padded_message[*padded_len - 1 - i] = (original_len_bits >> (8 * i)) & 0xFF;
	}

	return padded_message;
}

S32 md5()
{
	string src_file;
	bool quiet;
	bool reverse;

	{ /* options */
		S32 opt;

		quiet = FALSE;

		while ((opt = ft_getopt(ft_argc, (const_string *)ft_argv, "pqrs")))
		{
			switch (opt)
			{
			case 'p':
				break;

			case 'q':
				quiet = TRUE;
				break;

			case 'r':
				reverse = TRUE;
				break;

			case 's':
				break;

			case '?':
			default:
				print_help();
				return 1;
			}
		}

		ft_optind++; /* skip 'md5' */
	}

	char data[] = "42 is nice";
	U64 data_len = sizeof(data);

	/*
	x = [ data | 0x80 | zeros | original length in bits (8 bytes) ]
	len(x) % 64 == 0
	*/
	U8 buf[64];
	U64 data_len2 = data_len + 1 /* last bit */ + 8 /* original len */;
	U8 rem = data_len2 % 64;
	U64 total_len = data_len2 + (rem <= 56 ? 56 - rem : 56 + (64 - rem));

	ft_memcpy(buf, data, data_len);
	
}

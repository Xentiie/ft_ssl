/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:02:03 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/16 21:25:46 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define SHA1_BUFSIZE 64
#define SHA1HashSize 20
#define SHA1HashSizeBits 160

#define MIN(x, y) ((x) < (y) ? (x) : (y))

struct s_sha1_state
{
	U32 state[SHA1HashSize / 4];

	U32 length_hi;
	U32 length_lo;

	U16 msg_blk_i;
	U8 buffer[SHA1_BUFSIZE];
};

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static U32 addTemp;
#define SHA1AddLength(state, length)                     \
	(addTemp = (state)->length_lo,                       \
	 (state)->corrupted =                                \
		 (((state)->length_lo += (length)) < addTemp) && \
				 (++(state)->length_hi == 0)             \
			 ? shaInputTooLong                           \
			 : (state)->corrupted)

#define USE_MODIFIED_MACROS
#ifndef USE_MODIFIED_MACROS
#define SHA_Ch(x, y, z) (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA_Maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#else /* USE_MODIFIED_MACROS */
/*
 * The following definitions are equivalent and potentially faster.
 */

#define SHA_Ch(x, y, z) (((x) & ((y) ^ (z))) ^ (z))
#define SHA_Maj(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))

#endif /* USE_MODIFIED_MACROS */

#define SHA_Parity(x, y, z) ((x) ^ (y) ^ (z))

/* Local Function Prototypes */
static void SHA1ProcessMessageBlock(struct s_sha1_state *state);
static void SHA1Finalize(struct s_sha1_state *state, U8 Pad_Byte);
static void SHA1PadMessage(struct s_sha1_state *state, U8 Pad_Byte);

bool ft_sha1_init(struct s_sha1_state *state)
{
	if (!state)
		return FALSE;

	state->length_hi = 0;
	state->length_lo = 0;
	state->msg_blk_i = 0;

	state->state[0] = 0x67452301;
	state->state[1] = 0xEFCDAB89;
	state->state[2] = 0x98BADCFE;
	state->state[3] = 0x10325476;
	state->state[4] = 0xC3D2E1F0;
	return TRUE;
}

bool ft_sha1_update(struct s_sha1_state *state, const U8 *data, U64 len)
{
	printf("UPDATE\n");
	if (UNLIKELY(state == NULL) || UNLIKELY(data == NULL))
		return FALSE;
	if (UNLIKELY(len == 0))
		return TRUE;

	//	do
	//	{
	//		ft_memcpy(state->buffer + state->msg_blk_i, data, MIN(len, SHA1_BUFSIZE - state->msg_blk_i));
	//
	//	} while (len);

	while (len--)
	{
		state->buffer[state->msg_blk_i++] = *data;

		addTemp = (state)->length_lo;
		state->length_lo += 8;
		if (state->length_lo < addTemp)
			state->length_hi++;

		if (state->msg_blk_i == SHA1_BUFSIZE)
			SHA1ProcessMessageBlock(state);

	for (U8 i = 0; i < 5; i++)
	{
		printf("update: %#x\n", state->state[i]);
	}

		data++;
	}

	return TRUE;
}

bool ft_sha1_final_bits(struct s_sha1_state *state, U8 message_bits, U32 length)
{
	printf("FINAL BITS\n");
	static U8 masks[8] = {
		/* 0 00000000 */ 0x00,
		/* 1 10000000 */ 0x80,
		/* 2 11000000 */ 0xC0,
		/* 3 11100000 */ 0xE0,
		/* 4 11110000 */ 0xF0,
		/* 5 11111000 */ 0xF8,
		/* 6 11111100 */ 0xFC,
		/* 7 11111110 */ 0xFE};

	static U8 markbit[8] = {
		/* 0 10000000 */ 0x80,
		/* 1 01000000 */ 0x40,
		/* 2 00100000 */ 0x20,
		/* 3 00010000 */ 0x10,
		/* 4 00001000 */ 0x08,
		/* 5 00000100 */ 0x04,
		/* 6 00000010 */ 0x02,
		/* 7 00000001 */ 0x01};

	if (UNLIKELY(state == NULL) || UNLIKELY(length >= 8))
		return FALSE;
	if (UNLIKELY(length == 0))
		return TRUE;

	addTemp = (state)->length_lo;
	state->length_lo += 8;
	if (state->length_lo < addTemp)
		state->length_hi++;
	SHA1Finalize(state, (U8)((message_bits & masks[length]) | markbit[length]));

	return TRUE;
}

bool ft_sha1_final(struct s_sha1_state *state, U8 digest[SHA1HashSize])
{
	printf("FINAL\n");
	int i;

	if (UNLIKELY(state == NULL) || UNLIKELY(digest == NULL))
		return FALSE;


	for (i = 0; i < 5; i++)
	{
		printf("before final: %#x\n", state->state[i]);
	}
	SHA1Finalize(state, 0x80);
	for (i = 0; i < 5; i++)
	{
		printf("final: %#x\n", state->state[i]);
	}

	for (i = 0; i < SHA1HashSize; ++i)
		digest[i] = (U8)(state->state[i >> 2] >> (8 * (3 - (i & 0x03))));
	return TRUE;
}

/*
 * SHA1ProcessMessageBlock
 *
 * Description:
 *   This helper function will process the next 512 bits of the
 *   message stored in the Message_Block array.
 *
 * Parameters:
 *   state: [in/out]
 *     The SHA state to update.
 *
 * Returns:
 *   Nothing.
 *
 * Comments:
 *   Many of the variable names in this code, especially the
 *   single character names, were used because those were the
 *   names used in the Secure Hash Standard.
 */
static void SHA1ProcessMessageBlock(struct s_sha1_state *state)
{
	/* Constants defined in FIPS 180-3, section 4.2.1 */
	const U32 K[4] = {
		0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

	int t;			   /* Loop counter */
	U32 temp;		   /* Temporary word value */
	U32 W[80];		   /* Word sequence */
	U32 A, B, C, D, E; /* Word buffers */

	/*
	 * Initialize the first 16 words in the array W
	 */
	for (t = 0; t < 16; t++)
	{
		W[t] = ((U32)state->buffer[t * 4]) << 24;
		W[t] |= ((U32)state->buffer[t * 4 + 1]) << 16;
		W[t] |= ((U32)state->buffer[t * 4 + 2]) << 8;
		W[t] |= ((U32)state->buffer[t * 4 + 3]);
	}

	for (t = 16; t < 80; t++)
		W[t] = ROTATE_LEFT(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);

	for (int j = 0; j < 80; j++)
		printf("	%#x\n", W[j]);

	A = state->state[0];
	B = state->state[1];
	C = state->state[2];
	D = state->state[3];
	E = state->state[4];

	for (t = 0; t < 20; t++)
	{
		temp = ROTATE_LEFT(A, 5) + SHA_Ch(B, C, D) + E + W[t] + K[0];
		E = D;
		D = C;
		C = ROTATE_LEFT(B, 30);
		B = A;
		A = temp;
	}

	for (t = 20; t < 40; t++)
	{
		temp = ROTATE_LEFT(A, 5) + SHA_Parity(B, C, D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = ROTATE_LEFT(B, 30);
		B = A;
		A = temp;
	}

	for (t = 40; t < 60; t++)
	{
		temp = ROTATE_LEFT(A, 5) + SHA_Maj(B, C, D) + E + W[t] + K[2];
		E = D;
		D = C;
		C = ROTATE_LEFT(B, 30);
		B = A;
		A = temp;
	}

	for (t = 60; t < 80; t++)
	{
		temp = ROTATE_LEFT(A, 5) + SHA_Parity(B, C, D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = ROTATE_LEFT(B, 30);
		B = A;
		A = temp;
	}

	state->state[0] += A;
	state->state[1] += B;
	state->state[2] += C;
	state->state[3] += D;
	state->state[4] += E;
	state->msg_blk_i = 0;
}

/*
 * SHA1Finalize
 *
 * Description:
 *   This helper function finishes off the digest calculations.
 *
 * Parameters:
 *   state: [in/out]
 *     The SHA state to update.
 *   Pad_Byte: [in]
 *     The last byte to add to the message block before the 0-padding
 *     and length.  This will contain the last bits of the message
 *     followed by another single bit.  If the message was an
 *     exact multiple of 8-bits long, Pad_Byte will be 0x80.
 *
 * Returns:
 *   sha Error Code.
 *
 */
static void SHA1Finalize(struct s_sha1_state *state, U8 Pad_Byte)
{
	int i;
	SHA1PadMessage(state, Pad_Byte);
	/* message may be sensitive, clear it out */
	for (i = 0; i < SHA1_BUFSIZE; ++i)
		state->buffer[i] = 0;
	state->length_hi = 0; /* and clear length */
	state->length_lo = 0;
}

/*
 * SHA1PadMessage
 *
 * Description:
 *   According to the standard, the message must be padded to the next
 *   even multiple of 512 bits.  The first padding bit must be a '1'.
 *   The last 64 bits represent the length of the original message.
 *   All bits in between should be 0.  This helper function will pad
 *   the message according to those rules by filling the Message_Block
 *   array accordingly.  When it returns, it can be assumed that the
 *   message digest has been computed.
 *
 * Parameters:
 *   state: [in/out]
 *     The state to pad.
 *   Pad_Byte: [in]
 *     The last byte to add to the message block before the 0-padding
 *     and length.  This will contain the last bits of the message
 *     followed by another single bit.  If the message was an
 *     exact multiple of 8-bits long, Pad_Byte will be 0x80.
 *
 * Returns:
 *   Nothing.
 */
static void SHA1PadMessage(struct s_sha1_state *state, U8 Pad_Byte)
{
	/*
	 * Check to see if the current message block is too small to hold
	 * the initial padding bits and length.  If so, we will pad the
	 * block, process it, and then continue padding into a second
	 * block.
	 */
	if (state->msg_blk_i >= (SHA1_BUFSIZE - 8))
	{
		state->buffer[state->msg_blk_i++] = Pad_Byte;
		while (state->msg_blk_i < SHA1_BUFSIZE)
			state->buffer[state->msg_blk_i++] = 0;

		SHA1ProcessMessageBlock(state);
	}
	else
		state->buffer[state->msg_blk_i++] = Pad_Byte;

	while (state->msg_blk_i < (SHA1_BUFSIZE - 8))
		state->buffer[state->msg_blk_i++] = 0;

	/*
	 * Store the message length as the last 8 octets
	 */
	printf("%u %u\n", state->length_hi, state->length_lo);
	state->buffer[56] = (U8)(state->length_hi >> 24);
	state->buffer[57] = (U8)(state->length_hi >> 16);
	state->buffer[58] = (U8)(state->length_hi >> 8);
	state->buffer[59] = (U8)(state->length_hi);
	state->buffer[60] = (U8)(state->length_lo >> 24);
	state->buffer[61] = (U8)(state->length_lo >> 16);
	state->buffer[62] = (U8)(state->length_lo >> 8);
	state->buffer[63] = (U8)(state->length_lo);

	SHA1ProcessMessageBlock(state);
}

S32 sha1()
{
	struct s_hash_src *src;
	struct s_sha1_state state;
	U8 digest[SHA1HashSize];

	src = g_sources;
	while (src)
	{
		ft_sha1_init(&state);
		ft_sha1_update(&state, src->content, src->content_len);
		ft_sha1_final(&state, digest);

		print_result(src, digest, sizeof(digest));

		src = src->next;
	}

	return 0;
}
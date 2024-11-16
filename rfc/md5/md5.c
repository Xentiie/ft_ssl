/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:10:31 by reclaire          #+#    #+#             */
/*   Updated: 2024/11/09 02:10:31 by reclaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.h"
#include "md5.h"
#include <math.h>

/*
Constants for MD5Transform routine.
*/


static void md5_transform(U32[4], U8[64]);
/* echanger pr memcpy et tester big endian */
static void encode(U8 *dst, U32 *src, U32 srclen);
static void decode(U32 *dst, U8 *src, U32 srclen);

/*
   We first define four auxiliary functions that each take as input
   three 32-bit words and produce as output one 32-bit word.

		  F(X,Y,Z) = XY v not(X) Z
		  G(X,Y,Z) = XZ v Y not(Z)
		  H(X,Y,Z) = X xor Y xor Z
		  I(X,Y,Z) = Y xor (X v not(Z))
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits. */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/*
FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac)                   \
	{                                              \
		(a) += F((b), (c), (d)) + (x) + (U32)(ac); \
		(a) = ROTATE_LEFT((a), (s));               \
		(a) += (b);                                \
	}
#define GG(a, b, c, d, x, s, ac)                   \
	{                                              \
		(a) += G((b), (c), (d)) + (x) + (U32)(ac); \
		(a) = ROTATE_LEFT((a), (s));               \
		(a) += (b);                                \
	}
#define HH(a, b, c, d, x, s, ac)                   \
	{                                              \
		(a) += H((b), (c), (d)) + (x) + (U32)(ac); \
		(a) = ROTATE_LEFT((a), (s));               \
		(a) += (b);                                \
	}
#define II(a, b, c, d, x, s, ac)                   \
	{                                              \
		(a) += I((b), (c), (d)) + (x) + (U32)(ac); \
		(a) = ROTATE_LEFT((a), (s));               \
		(a) += (b);                                \
	}

/* MD5 initialization. Begins an MD5 operation, writing a new state. */
void md5_init(struct s_md5_state *state)
{
	state->count[0] = state->count[1] = 0;
	state->state[0] = 0x67452301;
	state->state[1] = 0xefcdab89;
	state->state[2] = 0x98badcfe;
	state->state[3] = 0x10325476;
}

/*
MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
state.
*/
void md5_update(struct s_md5_state *state, U8 *input, U32 len)
{
	U32 i;
	U32 index;
	U32 bits_len;
	U32 part_len;

	index = (U32)((state->count[0] / 8) % 64);

	bits_len = len * 8;
	/* Update number of bits */
	state->count[0] += bits_len;
	if (state->count[0] < bits_len)
		state->count[1] += 1;
	state->count[1] += ((U32)len >> 29);

	part_len = 64 - index;

	/* Transform as many times as possible. */
	if (len >= part_len)
	{
		ft_memcpy((U8 *)&state->buffer[index], (U8 *)input, part_len);
		md5_transform(state->state, state->buffer);

		for (i = part_len; i + 63 < len; i += 64)
			md5_transform(state->state, &input[i]);

		index = 0;
	}
	else
		i = 0;

	/* Buffer remaining input */
	ft_memcpy((U8 *)&state->buffer[index], (U8 *)&input[i],
			  len - i);
}

/*
MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the state.
*/
void md5_final(struct s_md5_state *state, U8 digest[16])
{
	static const U8 _padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	U8 bits[8];
	U32 index;
	U32 pad_len;

	/* Save number of bits */
	encode(bits, state->count, 8);

	/* Pad out to 56 mod 64. */
	index = (U32)((state->count[0] >> 3) & 0x3f);
	pad_len = (index < 56) ? (56 - index) : (120 - index);
	md5_update(state, _padding, pad_len);

	/* Append length (before padding) */
	md5_update(state, bits, 8);
	/* Store state in digest */
	encode(digest, state->state, 16);

	/* Zeroize sensitive information. */
	ft_bzero((U8 *)state, sizeof(struct s_md5_state));
}

/* MD5 basic transformation. Transforms state based on block. */
static void md5_transform(U32 state[4], U8 block[64])
{
	U32 a = state[0];
	U32 b = state[1];
	U32 c = state[2];
	U32 d = state[3];
	U32 x[16];

	decode(x, block, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], 7, 0xd76aa478);	/* 1 */
	FF(d, a, b, c, x[1], 12, 0xe8c7b756);	/* 2 */
	FF(c, d, a, b, x[2], 17, 0x242070db);	/* 3 */
	FF(b, c, d, a, x[3], 22, 0xc1bdceee);	/* 4 */
	FF(a, b, c, d, x[4], 7, 0xf57c0faf);	/* 5 */
	FF(d, a, b, c, x[5], 12, 0x4787c62a);	/* 6 */
	FF(c, d, a, b, x[6], 17, 0xa8304613);	/* 7 */
	FF(b, c, d, a, x[7], 22, 0xfd469501);	/* 8 */
	FF(a, b, c, d, x[8], 7, 0x698098d8);	/* 9 */
	FF(d, a, b, c, x[9], 12, 0x8b44f7af);	/* 10 */
	FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], 5, 0xf61e2562);	/* 17 */
	GG(d, a, b, c, x[6], 9, 0xc040b340);	/* 18 */
	GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);	/* 20 */
	GG(a, b, c, d, x[5], 5, 0xd62f105d);	/* 21 */
	GG(d, a, b, c, x[10], 9, 0x2441453);	/* 22 */
	GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);	/* 24 */
	GG(a, b, c, d, x[9], 5, 0x21e1cde6);	/* 25 */
	GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], 14, 0xf4d50d87);	/* 27 */
	GG(b, c, d, a, x[8], 20, 0x455a14ed);	/* 28 */
	GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], 9, 0xfcefa3f8);	/* 30 */
	GG(c, d, a, b, x[7], 14, 0x676f02d9);	/* 31 */
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], 4, 0xfffa3942);	/* 33 */
	HH(d, a, b, c, x[8], 11, 0x8771f681);	/* 34 */
	HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], 4, 0xa4beea44);	/* 37 */
	HH(d, a, b, c, x[4], 11, 0x4bdecfa9);	/* 38 */
	HH(c, d, a, b, x[7], 16, 0xf6bb4b60);	/* 39 */
	HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], 11, 0xeaa127fa);	/* 42 */
	HH(c, d, a, b, x[3], 16, 0xd4ef3085);	/* 43 */
	HH(b, c, d, a, x[6], 23, 0x4881d05);	/* 44 */
	HH(a, b, c, d, x[9], 4, 0xd9d4d039);	/* 45 */
	HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], 23, 0xc4ac5665);	/* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], 6, 0xf4292244);	/* 49 */
	II(d, a, b, c, x[7], 10, 0x432aff97);	/* 50 */
	II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], 21, 0xfc93a039);	/* 52 */
	II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], 10, 0x8f0ccc92);	/* 54 */
	II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], 21, 0x85845dd1);	/* 56 */
	II(a, b, c, d, x[8], 6, 0x6fa87e4f);	/* 57 */
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], 15, 0xa3014314);	/* 59 */
	II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], 6, 0xf7537e82);	/* 61 */
	II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], 15, 0x2ad7d2bb);	/* 63 */
	II(b, c, d, a, x[9], 21, 0xeb86d391);	/* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information. */
	ft_bzero((U8 *)x, sizeof(x));
}

/*
encodes input (U32) into output (U8). Assumes len is
a multiple of 4.
*/
static void encode(U8 *output, U32 *input, U32 len)
{
	U32 i;
	U32 j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j] = (U8)(input[i] & 0xff);
		output[j + 1] = (U8)((input[i] >> 8) & 0xff);
		output[j + 2] = (U8)((input[i] >> 16) & 0xff);
		output[j + 3] = (U8)((input[i] >> 24) & 0xff);
	}
}

/*
decodes input (U8) into output (U32). Assumes len is
a multiple of 4.
*/
static void decode(U32 *output, U8 *input, U32 len)
{
	U32 i;
	U32 j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[i] = ((U32)input[j]) | (((U32)input[j + 1]) << 8) |
					(((U32)input[j + 2]) << 16) | (((U32)input[j + 3]) << 24);
	}
}

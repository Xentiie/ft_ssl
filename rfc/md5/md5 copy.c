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

static void md5_transform(U32 state[4], U8 block[64]);
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

#if 0

void md5_rfc(U32 state[4], U8 *input, U64 len)
{
	S32 T[64];
	for (U8 i = 0; i < 64; i++)
		T[i] = 4294967296 * abs(sin(i));

	/* Process each 16-word block. */
	for (U64 i = 0; i < len / 16; i++)
	{
		S32 X[16];

		/* Copy block i into X. */
		for (U8 j = 0; j < 16; j++)
			X[j] = input[i * 16 + j];

		/* Save A as AA, B as BB, C as CC, and D as DD. */
		U32 A = state[0];
		U32 B = state[1];
		U32 C = state[2];
		U32 D = state[3];
		U32 AA = A;
		U32 BB = B;
		U32 CC = C;
		U32 DD = D;

		/* Round 1. */
		/* Let [abcd k s i] denote the operation
			 a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s). */
#define OP(a, b, c, d, k, s, i) a = b + ROTATE_LEFT((a + F(b, c, d) + X[k] + T[i]), s)
		/* Do the following 16 operations. */
		OP(A, B, C, D, 0, 7, 1);
		OP(D, A, B, C, 1, 12, 2);
		OP(C, D, A, B, 2, 17, 3);
		OP(B, C, D, A, 3, 22, 4);
		OP(A, B, C, D, 4, 7, 5);
		OP(D, A, B, C, 5, 12, 6);
		OP(C, D, A, B, 6, 17, 7);
		OP(B, C, D, A, 7, 22, 8);
		OP(A, B, C, D, 8, 7, 9);
		OP(D, A, B, C, 9, 12, 10);
		OP(C, D, A, B, 10, 17, 11);
		OP(B, C, D, A, 11, 22, 12);
		OP(A, B, C, D, 12, 7, 13);
		OP(D, A, B, C, 13, 12, 14);
		OP(C, D, A, B, 14, 17, 15);
		OP(B, C, D, A, 15, 22, 16);

		/*

	#define FF(a, b, c, d, x, s, ac)                   \
		{                                              \
			(a) += F((b), (c), (d)) + (x) + (U32)(ac); \
			(a) = ROTATE_LEFT((a), (s));               \
			(a) += (b);                                \
		}
		FF(a, b, c, d, x[0], 7, 0xd76aa478);

		==

	#define OP(a, b, c, d, k, s, i) a = b + ROTATE_LEFT((a + F(b,c,d) + X[k] + T[i]), s)
		OP(A, B, C, D,  0,  7,  1);

		*/

		/* Round 2. */
		/* Let [abcd k s i] denote the operation
			 a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s). */
		/* Do the following 16 operations. */
		//[ABCD  1  5 17]  [DABC  6  9 18]  [CDAB 11 14 19]  [BCDA  0 20 20]
		//[ABCD  5  5 21]  [DABC 10  9 22]  [CDAB 15 14 23]  [BCDA  4 20 24]
		//[ABCD  9  5 25]  [DABC 14  9 26]  [CDAB  3 14 27]  [BCDA  8 20 28]
		//[ABCD 13  5 29]  [DABC  2  9 30]  [CDAB  7 14 31]  [BCDA 12 20 32]

		/* Round 3. */
		/* Let [abcd k s t] denote the operation
			 a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s). */
		/* Do the following 16 operations. */
		//[ABCD  5  4 33]  [DABC  8 11 34]  [CDAB 11 16 35]  [BCDA 14 23 36]
		//[ABCD  1  4 37]  [DABC  4 11 38]  [CDAB  7 16 39]  [BCDA 10 23 40]
		//[ABCD 13  4 41]  [DABC  0 11 42]  [CDAB  3 16 43]  [BCDA  6 23 44]
		//[ABCD  9  4 45]  [DABC 12 11 46]  [CDAB 15 16 47]  [BCDA  2 23 48]

		/* Round 4. */
		/* Let [abcd k s t] denote the operation
			 a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s). */
		/* Do the following 16 operations. */
		//[ABCD  0  6 49]  [DABC  7 10 50]  [CDAB 14 15 51]  [BCDA  5 21 52]
		//[ABCD 12  6 53]  [DABC  3 10 54]  [CDAB 10 15 55]  [BCDA  1 21 56]
		//[ABCD  8  6 57]  [DABC 15 10 58]  [CDAB  6 15 59]  [BCDA 13 21 60]
		//[ABCD  4  6 61]  [DABC 11 10 62]  [CDAB  2 15 63]  [BCDA  9 21 64]

		/* Then perform the following additions. (That is increment each
		   of the four registers by the value it had before this block
		   was started.) */
		// A = A + AA
		// B = B + BB
		// C = C + CC
		// D = D + DD
	}
}
#endif

/* MD5 initialization. Begins an MD5 operation, writing a new state. */
void md5_init(struct s_md5_state *state)
{
	state->count[0] = state->count[1] = 0;
	/*
	 A four-word buffer (A,B,C,D) is used to compute the message digest.
	   Here each of A, B, C, D is a 32-bit register. These registers are
	   initialized to the following values in hexadecimal, low-order bytes
	   first):
			  word A: 01 23 45 67
			  word B: 89 ab cd ef
			  word C: fe dc ba 98
			  word D: 76 54 32 10
	*/
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
	U32 partLen;

#if 1
	/* Compute number of bytes mod 64 */
	index = (U32)((state->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((state->count[0] += ((U32)len << 3)) < ((U32)len << 3))
		state->count[1]++;
	state->count[1] += ((U32)len >> 29);
#else
	/* Compute number of bytes mod 64 */
	index = (U32)((state->count[0] / 8) % 64);

	U32 bits_len = len * 8;
	/* Update number of bits */
	state->count[0] += bits_len;
	if (state->count[0] < bits_len)
		state->count[1] += 1;
	state->count[1] += ((U32)len >> 29);
#endif

	partLen = 64 - index;

	/* Transform as many times as possible.
	 */
	if (len >= partLen)
	{
		ft_memcpy((U8 *)&state->buffer[index], (U8 *)input, partLen);
		md5_transform(state->state, state->buffer);

		for (i = partLen; i + 63 < len; i += 64)
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
	U32 padLen;

	/* Save number of bits */
	encode(bits, state->count, 8);

	/* Pad out to 56 mod 64. */
	index = (U32)((state->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	md5_update(state, (U8 *)_padding, padLen);

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
	FF(a, b, c, d, x[0], 7, 0xd76aa478);   /* 1 */
	FF(d, a, b, c, x[1], 12, 0xe8c7b756);  /* 2 */
	FF(c, d, a, b, x[2], 17, 0x242070db);  /* 3 */
	FF(b, c, d, a, x[3], 22, 0xc1bdceee);  /* 4 */
	FF(a, b, c, d, x[4], 7, 0xf57c0faf);   /* 5 */
	FF(d, a, b, c, x[5], 12, 0x4787c62a);  /* 6 */
	FF(c, d, a, b, x[6], 17, 0xa8304613);  /* 7 */
	FF(b, c, d, a, x[7], 22, 0xfd469501);  /* 8 */
	FF(a, b, c, d, x[8], 7, 0x698098d8);   /* 9 */
	FF(d, a, b, c, x[9], 12, 0x8b44f7af);  /* 10 */
	FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], 7, 0x6b901122);  /* 13 */
	FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], 5, 0xf61e2562);   /* 17 */
	GG(d, a, b, c, x[6], 9, 0xc040b340);   /* 18 */
	GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);  /* 20 */
	GG(a, b, c, d, x[5], 5, 0xd62f105d);   /* 21 */
	GG(d, a, b, c, x[10], 9, 0x2441453);   /* 22 */
	GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);  /* 24 */
	GG(a, b, c, d, x[9], 5, 0x21e1cde6);   /* 25 */
	GG(d, a, b, c, x[14], 9, 0xc33707d6);  /* 26 */
	GG(c, d, a, b, x[3], 14, 0xf4d50d87);  /* 27 */
	GG(b, c, d, a, x[8], 20, 0x455a14ed);  /* 28 */
	GG(a, b, c, d, x[13], 5, 0xa9e3e905);  /* 29 */
	GG(d, a, b, c, x[2], 9, 0xfcefa3f8);   /* 30 */
	GG(c, d, a, b, x[7], 14, 0x676f02d9);  /* 31 */
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], 4, 0xfffa3942);   /* 33 */
	HH(d, a, b, c, x[8], 11, 0x8771f681);  /* 34 */
	HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], 32, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], 4, 0xa4beea44);   /* 37 */
	HH(d, a, b, c, x[4], 11, 0x4bdecfa9);  /* 38 */
	HH(c, d, a, b, x[7], 16, 0xf6bb4b60);  /* 39 */
	HH(b, c, d, a, x[10], 32, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], 4, 0x289b7ec6);  /* 41 */
	HH(d, a, b, c, x[0], 11, 0xeaa127fa);  /* 42 */
	HH(c, d, a, b, x[3], 16, 0xd4ef3085);  /* 43 */
	HH(b, c, d, a, x[6], 32, 0x4881d05);   /* 44 */
	HH(a, b, c, d, x[9], 4, 0xd9d4d039);   /* 45 */
	HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], 32, 0xc4ac5665);  /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], 6, 0xf4292244);   /* 49 */
	II(d, a, b, c, x[7], 10, 0x432aff97);  /* 50 */
	II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], 21, 0xfc93a039);  /* 52 */
	II(a, b, c, d, x[12], 6, 0x655b59c3);  /* 53 */
	II(d, a, b, c, x[3], 10, 0x8f0ccc92);  /* 54 */
	II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], 21, 0x85845dd1);  /* 56 */
	II(a, b, c, d, x[8], 6, 0x6fa87e4f);   /* 57 */
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], 15, 0xa3014314);  /* 59 */
	II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], 6, 0xf7537e82);   /* 61 */
	II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], 15, 0x2ad7d2bb);  /* 63 */
	II(b, c, d, a, x[9], 21, 0xeb86d391);  /* 64 */

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

	len *= 4;
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

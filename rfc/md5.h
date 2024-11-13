/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software.
 */

/* MD5 context. */

#include "global.h"

struct s_md5_state
{
  U32 state[4];  /* state (ABCD) */
  U32 count[2];  /* number of bits, modulo 2^64 (lsb first) */
  U8 buffer[64]; /* input buffer */
};

void md5_init(struct s_md5_state *state);
void md5_update(struct s_md5_state *state, U8 *input, U32 inputLen);
void md5_final(struct s_md5_state *state, U8 digest[16]);
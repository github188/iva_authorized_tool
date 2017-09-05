/*
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991.
 * All rights reserved.
 *
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 *
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD5 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 *
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 */

#ifndef _MD5_H
#define _MD5_H

// Fairly raw MD5 code .. I need to clean it up when I'm bored ;)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int  state[4];        /* state (ABCD) */
    unsigned int  count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];      /* input buffer */
} MD5_CTX;

void MD5Init( MD5_CTX* );
void MD5Update( MD5_CTX *, const unsigned char *, unsigned int );
void MD5Final( unsigned char[16], MD5_CTX* );

char* MD5End(MD5_CTX *ctx, char *p);
char* MD5Data(char *buf, const unsigned char *data, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif // ndef _MD5_H


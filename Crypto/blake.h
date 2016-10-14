/*
   BLAKE reference C implementation

   Copyright (c) 2012 Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along with
   this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#ifndef _BLAKE_H
#define _BLAKE_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#if defined(__cplusplus)
extern "C"
{
#endif
#define U8TO32_BIG(p)					      \
  (((uint32_t)((p)[0]) << 24) | ((uint32_t)((p)[1]) << 16) |  \
   ((uint32_t)((p)[2]) <<  8) | ((uint32_t)((p)[3])      ))

#define U32TO8_BIG(p, v)				        \
  (p)[0] = (uint8_t)((v) >> 24); (p)[1] = (uint8_t)((v) >> 16); \
  (p)[2] = (uint8_t)((v) >>  8); (p)[3] = (uint8_t)((v)      );

#define U8TO64_BIG(p) \
  (((uint64_t)U8TO32_BIG(p) << 32) | (uint64_t)U8TO32_BIG((p) + 4))

#define U64TO8_BIG(p, v)		      \
  U32TO8_BIG((p),     (uint32_t)((v) >> 32)); \
  U32TO8_BIG((p) + 4, (uint32_t)((v)      ));

typedef struct
{
  uint64_t h[8], s[4], t[2];
  int buflen, nullt;
  uint8_t buf[128];
} BLAKE512_CTX;

// typedef BLAKE512_CTX BLAKE384_CTX;

void blake512_init( BLAKE512_CTX *S );
void blake512_final( BLAKE512_CTX *S, uint8_t *out );
void blake512_hash( uint8_t *out, const uint8_t *in, uint64_t inlen );
void blake512_update( BLAKE512_CTX *S, const uint8_t *in, uint64_t inlen );
void blake512_test();

#if defined(__cplusplus)
}
#endif

#endif

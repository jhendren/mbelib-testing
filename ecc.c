/*
 * Copyright (C) 2010 mbelib Author
 * GPG Key ID: 0xEA5EFE2C (9E7A 5527 9CDC EBF7 BF1B  D772 4F98 E863 EA5E FE2C)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <math.h>
#include "ecc_const.h"

void mbe_checkGolayBlock (unsigned int *block)
{
  static unsigned int i, syndrome, eccexpected, eccbits, databits;
  unsigned int mask, block_l = *block;

  mask = 0x400000l;
  eccexpected = 0;
  for (i = 0; i < 12; i++) {
      if ((block_l & mask) != 0l) {
          eccexpected ^= golayGenerator[i];
      }
      mask = mask >> 1;
  }

  eccbits =  (block_l & 0x7ffl);
  syndrome = eccexpected ^ eccbits;
  databits = (block_l >> 11);
  databits = databits ^ golayMatrix[syndrome];
  *block = databits;
}

unsigned int mbe_golay2312 (char *in, char *out)
{
  int i;
  unsigned int errs = 0, block = 0;

  for (i = 22; i >= 0; i--) {
      block = block << 1;
      block = block + in[i];
  }

  mbe_checkGolayBlock (&block);

  for (i = 22; i >= 11; i--) {
      out[i] = (block & 2048) >> 11;
      block = block << 1;
  }
  for (i = 10; i >= 0; i--) {
      out[i] = in[i];
  }

  for (i = 22; i >= 11; i--) {
      if (out[i] != in[i]) {
          errs++;
      }
  }
  return (errs);
}


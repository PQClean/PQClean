#ifndef Decode_H
#define Decode_H

#include <stdint.h>

/* Decode(R,s,M,len) */
/* assumes 0 < M[i] < 16384 */
/* produces 0 <= R[i] < M[i] */
extern void Decode(uint16_t *, const unsigned char *, const uint16_t *, long long);

#endif

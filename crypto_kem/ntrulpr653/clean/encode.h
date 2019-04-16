#ifndef Encode_H
#define Encode_H

#include <stdint.h>

/* Encode(s,R,M,len) */
/* assumes 0 <= R[i] < M[i] < 16384 */
extern void Encode(unsigned char *,const uint16_t *,const uint16_t *,long long);

#endif

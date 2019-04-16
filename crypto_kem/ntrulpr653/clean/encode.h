#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

/* PQCLEAN_NTRULPR653_CLEAN_Encode(s,R,M,len) */
/* assumes 0 <= R[i] < M[i] < 16384 */
extern void PQCLEAN_NTRULPR653_CLEAN_Encode(unsigned char *, const uint16_t *, const uint16_t *, long long);

#endif

#ifndef INT32_H
#define INT32_H

#include <stdint.h>

/*
assuming 1 <= m < 16384:
q = PQCLEAN_NTRULPR653_CLEAN_int32_div_uint14(x,m) means q = x/m
r = PQCLEAN_NTRULPR653_CLEAN_int32_mod_uint14(x,m) means r = x/m
int32_moddiv_uint14(&q,&r,x,m) means q = x/m, r = x%m
*/

extern int32_t PQCLEAN_NTRULPR653_CLEAN_int32_div_uint14(int32_t, uint16_t);
extern uint16_t PQCLEAN_NTRULPR653_CLEAN_int32_mod_uint14(int32_t, uint16_t);
extern void PQCLEAN_NTRULPR653_CLEAN_int32_divmod_uint14(int32_t *, uint16_t *, int32_t, uint16_t);

#endif

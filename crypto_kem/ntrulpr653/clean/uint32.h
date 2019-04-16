#ifndef UINT32_H
#define UINT32_H

/*
assuming 1 <= m < 16384:
q = PQCLEAN_NTRULPR653_CLEAN_uint32_div_uint14(x,m) means q = x/m
r = PQCLEAN_NTRULPR653_CLEAN_uint32_mod_uint14(x,m) means r = x/m
uint32_moddiv_uint14(&q,&r,x,m) means q = x/m, r = x%m
*/

extern uint32_t PQCLEAN_NTRULPR653_CLEAN_uint32_div_uint14(uint32_t, uint16_t);
extern uint16_t PQCLEAN_NTRULPR653_CLEAN_uint32_mod_uint14(uint32_t, uint16_t);
extern void PQCLEAN_NTRULPR653_CLEAN_uint32_divmod_uint14(uint32_t *, uint16_t *, uint32_t, uint16_t);

extern void PQCLEAN_NTRULPR653_CLEAN_uint32_sort(uint32_t *, int);

#endif

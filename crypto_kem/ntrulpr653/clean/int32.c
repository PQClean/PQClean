#include <stdint.h>

#include "int32.h"
#include "uint32.h"

void int32_divmod_uint14(int32_t *q, uint16_t *r, int32_t x, uint16_t m) {
    uint32_t uq, uq2;
    uint16_t ur, ur2;
    uint32_t mask;

    uint32_divmod_uint14(&uq, &ur, 0x80000000 + (uint32_t)x, m);
    uint32_divmod_uint14(&uq2, &ur2, 0x80000000, m);
    ur -= ur2;
    uq -= uq2;
    mask = -(uint32_t)(ur >> 15);
    ur += mask & m;
    uq += mask;
    *r = ur;
    *q = uq;
}

int32_t int32_div_uint14(int32_t x, uint16_t m) {
    int32_t q;
    uint16_t r;
    int32_divmod_uint14(&q, &r, x, m);
    return q;
}

uint16_t int32_mod_uint14(int32_t x, uint16_t m) {
    int32_t q;
    uint16_t r;
    int32_divmod_uint14(&q, &r, x, m);
    return r;
}

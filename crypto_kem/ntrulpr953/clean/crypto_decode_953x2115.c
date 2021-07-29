#include "crypto_decode_953x2115.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t

/*
CPU division instruction typically takes time depending on x.
This software is designed to take time independent of x.
Time still varies depending on m; user must ensure that m is constant.
Time also varies on CPUs where multiplication is variable-time.
There could be more CPU issues.
There could also be compiler issues.
*/

static void uint32_divmod_uint14(uint32 *q, uint16 *r, uint32 x, uint16 m) {
    uint32 v = 0x80000000;
    uint32 qpart;
    uint32 mask;

    v /= m;

    /* caller guarantees m > 0 */
    /* caller guarantees m < 16384 */
    /* vm <= 2^31 <= vm+m-1 */
    /* xvm <= 2^31 x <= xvm+x(m-1) */

    *q = 0;

    qpart = (uint32) ((x * (uint64)v) >> 31);
    /* 2^31 qpart <= xv <= 2^31 qpart + 2^31-1 */
    /* 2^31 qpart m <= xvm <= 2^31 qpart m + (2^31-1)m */
    /* 2^31 qpart m <= 2^31 x <= 2^31 qpart m + (2^31-1)m + x(m-1) */
    /* 0 <= 2^31 newx <= (2^31-1)m + x(m-1) */
    /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
    /* 0 <= newx <= (1-1/2^31)(2^14-1) + (2^32-1)((2^14-1)-1)/2^31 */

    x -= qpart * m;
    *q += qpart;
    /* x <= 49146 */

    qpart = (uint32) ((x * (uint64)v) >> 31);
    /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
    /* 0 <= newx <= m + 49146(2^14-1)/2^31 */
    /* 0 <= newx <= m + 0.4 */
    /* 0 <= newx <= m */

    x -= qpart * m;
    *q += qpart;
    /* x <= m */

    x -= m;
    *q += 1;
    mask = (~(x >> 31) + 1);
    x += mask & (uint32)m;
    *q += mask;
    /* x < m */

    *r = (uint16) x;
}

static uint16 uint32_mod_uint14(uint32 x, uint16 m) {
    uint32 q;
    uint16 r;
    uint32_divmod_uint14(&q, &r, x, m);
    return r;
}

void PQCLEAN_NTRULPR953_CLEAN_crypto_decode_953x2115(void *v, const unsigned char *s) {
    int16 *R0 = v;
    uint16 R1[477], R2[239], R3[120], R4[60], R5[30], R6[15], R7[8], R8[4], R9[2], R10[1];
    long long i;
    uint16 r0;
    uint32 r1, r2;

    s += PQCLEAN_NTRULPR953_CLEAN_crypto_decode_953x2115_STRBYTES;
    r1 = 0;
    r1 = (r1 << 8) | *--s;
    r1 = uint32_mod_uint14(r1, 124); /* needed only for invalid inputs */
    R10[0] = (uint16) r1;

    r2 = R10[0];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 3846);
    R9[0] = r0;
    r1 = uint32_mod_uint14(r1, 2107); /* needed only for invalid inputs */
    R9[1] = (uint16) r1;

    r2 = R9[1];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 15876);
    R8[2] = r0;
    r1 = uint32_mod_uint14(r1, 8694); /* needed only for invalid inputs */
    R8[3] = (uint16) r1;
    r2 = R9[0];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 15876);
    R8[0] = r0;
    r1 = uint32_mod_uint14(r1, 15876); /* needed only for invalid inputs */
    R8[1] = (uint16) r1;

    r2 = R8[3];
    uint32_divmod_uint14(&r1, &r0, r2, 126);
    R7[6] = r0;
    r1 = uint32_mod_uint14(r1, 69); /* needed only for invalid inputs */
    R7[7] = (uint16) r1;
    for (i = 2; i >= 0; --i) {
        r2 = R8[i];
        uint32_divmod_uint14(&r1, &r0, r2, 126);
        R7[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 126); /* needed only for invalid inputs */
        R7[2 * i + 1] = (uint16) r1;
    }

    R6[14] = R7[7];
    for (i = 6; i >= 0; --i) {
        r2 = R7[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 2863);
        R6[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 2863); /* needed only for invalid inputs */
        R6[2 * i + 1] = (uint16) r1;
    }

    r2 = R6[14];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 856);
    R5[28] = r0;
    r1 = uint32_mod_uint14(r1, 5227); /* needed only for invalid inputs */
    R5[29] = (uint16) r1;
    for (i = 13; i >= 0; --i) {
        r2 = R6[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 856);
        R5[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 856); /* needed only for invalid inputs */
        R5[2 * i + 1] = (uint16) r1;
    }

    r2 = R5[29];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 468);
    R4[58] = r0;
    r1 = uint32_mod_uint14(r1, 2859); /* needed only for invalid inputs */
    R4[59] = (uint16) r1;
    for (i = 28; i >= 0; --i) {
        r2 = R5[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 468);
        R4[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 468); /* needed only for invalid inputs */
        R4[2 * i + 1] = (uint16) r1;
    }

    r2 = R4[59];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 346);
    R3[118] = r0;
    r1 = uint32_mod_uint14(r1, 2115); /* needed only for invalid inputs */
    R3[119] = (uint16) r1;
    for (i = 58; i >= 0; --i) {
        r2 = R4[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 346);
        R3[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 346); /* needed only for invalid inputs */
        R3[2 * i + 1] = (uint16) r1;
    }

    R2[238] = R3[119];
    for (i = 118; i >= 0; --i) {
        r2 = R3[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 4761);
        R2[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 4761); /* needed only for invalid inputs */
        R2[2 * i + 1] = (uint16) r1;
    }

    R1[476] = R2[238];
    for (i = 237; i >= 0; --i) {
        r2 = R2[i];
        uint32_divmod_uint14(&r1, &r0, r2, 69);
        R1[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 69); /* needed only for invalid inputs */
        R1[2 * i + 1] = (uint16) r1;
    }

    R0[952] = (int16) (3 * R1[476] - 3171);
    for (i = 475; i >= 0; --i) {
        r2 = R1[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 2115);
        R0[2 * i] = (int16) (3 * r0 - 3171);
        r1 = uint32_mod_uint14(r1, 2115); /* needed only for invalid inputs */
        R0[2 * i + 1] = (int16) (3 * r1 - 3171);
    }
}

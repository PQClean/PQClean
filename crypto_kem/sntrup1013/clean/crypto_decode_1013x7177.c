#include "crypto_decode_1013x7177.h"

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

void PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013x7177(void *v, const unsigned char *s) {
    int16 *R0 = v;
    uint16 R1[507], R2[254], R3[127], R4[64], R5[32], R6[16], R7[8], R8[4], R9[2], R10[1];
    long long i;
    uint16 r0;
    uint32 r1, r2;

    s += PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013x7177_STRBYTES;
    r1 = 0;
    r1 = (r1 << 8) | *--s;
    r1 = (r1 << 8) | *--s;
    r1 = uint32_mod_uint14(r1, 274); /* needed only for invalid inputs */
    R10[0] = (uint16) r1;

    r2 = R10[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 91);
    R9[0] = r0;
    r1 = uint32_mod_uint14(r1, 769); /* needed only for invalid inputs */
    R9[1] = (uint16) r1;

    r2 = R9[1];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 152);
    R8[2] = r0;
    r1 = uint32_mod_uint14(r1, 1294); /* needed only for invalid inputs */
    R8[3] = (uint16) r1;
    r2 = R9[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 152);
    R8[0] = r0;
    r1 = uint32_mod_uint14(r1, 152); /* needed only for invalid inputs */
    R8[1] = (uint16) r1;

    r2 = R8[3];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 197);
    R7[6] = r0;
    r1 = uint32_mod_uint14(r1, 1681); /* needed only for invalid inputs */
    R7[7] = (uint16) r1;
    for (i = 2; i >= 0; --i) {
        r2 = R8[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 197);
        R7[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 197); /* needed only for invalid inputs */
        R7[2 * i + 1] = (uint16) r1;
    }

    r2 = R7[7];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 3586);
    R6[14] = r0;
    r1 = uint32_mod_uint14(r1, 120); /* needed only for invalid inputs */
    R6[15] = (uint16) r1;
    for (i = 6; i >= 0; --i) {
        r2 = R7[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 3586);
        R6[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 3586); /* needed only for invalid inputs */
        R6[2 * i + 1] = (uint16) r1;
    }

    r2 = R6[15];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 958);
    R5[30] = r0;
    r1 = uint32_mod_uint14(r1, 8200); /* needed only for invalid inputs */
    R5[31] = (uint16) r1;
    for (i = 14; i >= 0; --i) {
        r2 = R6[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 958);
        R5[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 958); /* needed only for invalid inputs */
        R5[2 * i + 1] = (uint16) r1;
    }

    r2 = R5[31];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 7921);
    R4[62] = r0;
    r1 = uint32_mod_uint14(r1, 265); /* needed only for invalid inputs */
    R4[63] = (uint16) r1;
    for (i = 30; i >= 0; --i) {
        r2 = R5[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 7921);
        R4[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 7921); /* needed only for invalid inputs */
        R4[2 * i + 1] = (uint16) r1;
    }

    R3[126] = R4[63];
    for (i = 62; i >= 0; --i) {
        r2 = R4[i];
        uint32_divmod_uint14(&r1, &r0, r2, 89);
        R3[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 89); /* needed only for invalid inputs */
        R3[2 * i + 1] = (uint16) r1;
    }

    r2 = R3[126];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 2414);
    R2[252] = r0;
    r1 = uint32_mod_uint14(r1, 7177); /* needed only for invalid inputs */
    R2[253] = (uint16) r1;
    for (i = 125; i >= 0; --i) {
        r2 = R3[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 2414);
        R2[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 2414); /* needed only for invalid inputs */
        R2[2 * i + 1] = (uint16) r1;
    }

    R1[506] = R2[253];
    for (i = 252; i >= 0; --i) {
        r2 = R2[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 786);
        R1[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 786); /* needed only for invalid inputs */
        R1[2 * i + 1] = (uint16) r1;
    }

    R0[1012] = (int16) (R1[506] - 3588);
    for (i = 505; i >= 0; --i) {
        r2 = R1[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 7177);
        R0[2 * i] = (int16) (r0 - 3588);
        r1 = uint32_mod_uint14(r1, 7177); /* needed only for invalid inputs */
        R0[2 * i + 1] = (int16) (r1 - 3588);
    }
}

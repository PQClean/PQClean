/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
/*
*
*  $File$
*
*  $Author$
*
*  $Date$
*
*  $Rev$
*
***********************************************************************************************************************/
/* Floating-point operations.
 * This file implements the non-inline functions declared in fpr.h, as well as the constants for FFT / iFFT. */

/* Custom floating-point implementation with integer arithmetics. We use IEEE-754 "binary64" format, with some
 * simplifications:
 *
 *   - Top bit is s = 1 for negative, 0 for positive.
 *   - Exponent e uses the next 11 bits (bits 52 to 62, inclusive).
 *   - Mantissa m uses the 52 low bits.
 *
 * Encoded value is, in general: (-1)^s * 2^(e-1023) * (1 + m*2^(-52)) i.e. the mantissa really is a 53-bit number
 * (less than 2.0, but not less than 1.0), but the top bit (equal to 1 by definition) is omitted in the encoding.
 *
 * In IEEE-754, there are some special values:
 *
 *   - If e = 2047, then the value is either an infinite (m = 0) or a NaN (m != 0).
 *   - If e = 0, then the value is either a zero (m = 0) or a subnormal, aka "denormalized number" (m != 0).
 *
 * Of these, we only need the zeros. The caller is responsible for not providing operands that would lead to infinites,
 * NaNs or subnormals. If inputs are such that values go out of range, then indeterminate values are returned (it would
 * still be deterministic, but no specific value may be relied upon).
 *
 * At the C level, the three parts are stored in a 64-bit unsigned word.
 *
 * One may note that a property of the IEEE-754 format is that order is preserved for positive values: if two positive
 * floating-point values x and y are such that x < y, then their respective encodings as _signed_ 64-bit integers i64(x)
 * and i64(y) will be such that i64(x) < i64(y). For negative values, order is reversed: if x < 0, * y < 0, and x < y,
 * then ia64(x) > ia64(y).
 *
 * IMPORTANT ASSUMPTIONS:
 * ======================
 *
 * For proper computations, and constant-time behaviour, we assume the following:
 *
 *   - 32x32->64 multiplication (unsigned) has an execution time that is independent of its operands. This is true of
 *     most modern x86 and ARM cores. Notable exceptions are the ARM Cortex M0, M0+ and M3 (in the M0 and M0+, this is
 *     done in software, so it depends on that routine), and the PowerPC cores from the G3/G4 lines. */
       /* polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" */ /*
 *     For more info, see: https://www.bearssl.org/ctmul.html */ /*
 *
 *   - Left-shifts and right-shifts of 32-bit values have an execution time which does not depend on the shifted value
 *     nor on the shift count. An historical exception is the Pentium IV, but most modern CPU have barrel shifters.
 *     Some small microcontrollers might have varying-time shifts (not the ARM Cortex M*, though).
 *
 *   - Right-shift of a signed negative value performs a sign extension. As per the C standard, this operation returns
 *     an implementation-defined result (this is NOT an "undefined behaviour"). On most/all systems, an arithmetic
 *     shift is performed, because this is what makes most sense. */

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Falcon_fpr.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static const uint64 C[] = {0x00000004741183A3u, 0x00000036548CFC06u, 0x0000024FDCBF140Au, 0x0000171D939DE045u,
                           0x0000D00CF58F6F84u, 0x000680681CF796E3u, 0x002D82D8305B0FEAu, 0x011111110E066FD0u,
                           0x0555555555070F00u, 0x155555555581FF00u, 0x400000000002B400u, 0x7FFFFFFFFFFF4800u,
                           0x8000000000000000u};

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static fpr    fpr_scaled(sint64 i, sint32 sc);
static uint64 fpr_ursh(uint64 x, sint32 n);
static sint64 fpr_irsh(sint64 x, sint32 n);
static uint64 fpr_ulsh(uint64 x, sint32 n);
static fpr    fpr_check_exponent(sint32 s, sint32 e, uint64 m);
static fpr    fpr_div(fpr x, fpr y);
static void   fpr_norm64(uint64 *mp, sint32 *ep);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/* For computations, we split values into an integral mantissa in the 2^54..2^55 range, and an (adjusted) exponent.
 * The lowest bit is "sticky" (it is set to 1 if any of the bits below it is 1); when re-encoding, the low two bits are
 * dropped, but may induce an increment in the value for proper rounding. */


/***********************************************************************************************************************
* Name:        fpr_ursh
*
* Description: Right-shift a 64-bit unsigned value by a possibly secret shift count. We assumed that the underlying
*              architecture had a barrel shifter for 32-bit shifts, but for 64-bit shifts on a 32-bit system, this will
*              typically invoke a software routine that is not necessarily constant-time; hence the function below.
*
* Arguments:   - uint64 x: t.b.d.
*              - sint32 n: Shift count n MUST be in the 0..63 range.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
static uint64 fpr_ursh(uint64 x, sint32 n)
{
    /* x_temp is used to avoid modifying the input. */
    uint64 x_temp = x;

    x_temp ^= (x_temp ^ (x_temp >> 32)) & (uint64)((sint64)((-1) * (sint64)((uint64)((uint64)n >> 5))));
    return x_temp >> ((uint64)n & 31u);
}

/***********************************************************************************************************************
* Name:        fpr_irsh
*
* Description: Right-shift a 64-bit signed value by a possibly secret shift count (see fpr_ursh() for the rationale).
*
* Arguments:   - sint64 x: t.b.d.
*              - sint32 n: Shift count n MUST be in the 0..63 range.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
static sint64 fpr_irsh(sint64 x, sint32 n)
{
    sint64 temp3 = 0;
    uint64 temp2 = 0;
    sint64 temp1 = ((-1) * (sint64)((uint64)((uint64)n >> 5)));

    /* x_temp is used to avoid modifying the input. */
    sint64 x_temp = x;

    if(x_temp >= 0)
    {
        temp2 = ((uint64)x_temp >> 32);
    }
    else
    {
        temp2 = ((uint64)x_temp >> 32) | 0xFFFFFFFF00000000u ;
    }

    temp3 = (sint64)((uint64)((uint64)x_temp ^ temp2));


    x_temp = (sint64)((uint64)((uint64)x_temp ^ (uint64)((uint64)temp3 & (uint64)temp1)));

    return (sint64)((uint64)((uint64)x_temp >> ((uint64)n & 31u)));
}

/***********************************************************************************************************************
* Name:        fpr_ulsh
*
* Description: Left-shift a 64-bit unsigned value by a possibly secret shift count (see fpr_ursh() for the rationale).
*
* Arguments:   - uint64 x: t.b.d.
*              - sint32 n: Shift count n MUST be in the 0..63 range.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
static uint64 fpr_ulsh(uint64 x, sint32 n)
{
    /* x_temp is used to avoid modifying the input. */
    uint64 x_temp = x;

    x_temp ^= (x_temp ^ (x_temp << 32u)) & (uint64)((sint64)((-1) * (sint64)((uint64)((uint64)n >> 5))));
    return x_temp << ((uint64)n & 31u);
}

/***********************************************************************************************************************
* Name:        fpr_check_exponent
*
* Description: Numerical value is (-1)^2 * m * 2^e. Exponents which are too low lead to value zero. If the exponent is
*              too large, the returned value is indeterminate.
*              If m = 0, then a zero is returned (using the provided sign).
*              If e < -1076, then a zero is returned (regardless of the value of m).
*              If e >= -1076 and e != 0, m must be within the expected range (2^54 to 2^55-1).
*
* Arguments:   - sint32 s: s = 0 or 1
*              - sint32 e: exponent e is "arbitrary"
*              - uint64 m: unbiased 2^54 <= m < 2^55
*
* Returns x
*
***********************************************************************************************************************/
static fpr fpr_check_exponent(sint32 s, sint32 e, uint64 m)
{
    fpr x = 0;
    uint32 t = 0;
    uint32 f = 0;
    uint32 temp = 0;

    /* e_temp and m_temp are used to avoid modifying the input. */
    sint32 e_temp = e;
    uint64 m_temp = m;

    /* If e >= -1076, then the value is "normal"; otherwise, it should be a subnormal, which we clamp down to zero. */
    e_temp += 1076;
    t = (uint32)e_temp >> 31;
    m_temp &= (uint64)t - 1u;

    /* If m = 0 then we want a zero; make e = 0 too, but conserve the sign. */
    t = (uint32)(m_temp >> 54u);
    e_temp = (sint32)((uint32)((uint32)e_temp & (uint32)((sint32)((-1)*(sint32)t))));

    /* The 52 mantissa bits come from m. Value m has its top bit set (unless it is a zero); we leave it "as is": the
     * top bit will increment the exponent by 1, except when m = 0, which is exactly what we want. */
    x = (((uint64)s << 63) | (m_temp >> 2)) + ((uint64)(uint32)e_temp << 52);

    /* Rounding: if the low three bits of m are 011, 110 or 111 then the value should be incremented to get the next
     * representable value. This implements the usual round-to-nearest rule (with preference to even values in case
     * of a tie). Note that the increment may make a carry spill into the exponent field, which is again exactly what
     * we want in that case. */
    f = (uint32)m_temp & 7u;
    temp = ((uint32)0xC8u >> f) & 1u;
    x += (fpr)temp;

    return x;
}

/***********************************************************************************************************************
* Name:        fpr_div
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*              - fpr y: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
static fpr fpr_div(fpr x, fpr y)
{
    uint64 xu = 0;
    uint64 yu = 0;
    uint64 q  = 0;
    uint64 q2 = 0;
    uint64 w  = 0;
    sint32 i  = 0;
    sint32 ex = 0;
    sint32 ey = 0;
    sint32 e  = 0;
    sint32 d  = 0;
    sint32 s  = 0;
    uint64 b  = 0;

    /* Extract mantissas of x and y. */
    xu = (x & (((uint64) 1 << 52) - 1u)) | ((uint64) 1 << 52);
    yu = (y & (((uint64) 1 << 52) - 1u)) | ((uint64) 1 << 52);

    /* Perform bit-by-bit division of xu by yu. We run it for 55 bits. */
    q = 0;

    for (i = 0; i < 55; i++)
    {
        /* If yu is less than or equal xu, then subtract it and push a 1 in the quotient; otherwise, leave xu unchanged
         * and push a 0. */
        b = ((xu - yu) >> 63) - 1u;
        xu -= b & yu;
        q |= b & 1u;
        xu <<= 1;
        q <<= 1;
    }

    /* We got 55 bits in the quotient, followed by an extra zero. We want that 56th bit to be "sticky": it should be a
     * 1 if and only if the remainder (xu) is non-zero. */
    q |= (xu | (uint64)((sint64)((-1) * (sint64)xu))) >> 63;

    /* Quotient is at most 2^56-1. Its top bit may be zero, but in that case the next-to-top bit will be a one, since
     * the initial xu and yu were both in the 2^52..2^53-1 range.  We perform a conditional shift to normalize q to the
     * 2^54..2^55-1 range (with the bottom bit being sticky). */
    q2 = (q >> 1) | (q & 1u);
    w = q >> 55;
    q ^= (q ^ q2) & (uint64)((sint64)((-1) * (sint64)w));

    /* Extract exponents to compute the scaling factor:
     *   - Each exponent is biased and we scaled them up by 52 bits; but these biases will cancel out.
     *   - The division loop produced a 55-bit shifted result, so we must scale it down by 55 bits.
     *   - If w = 1, we right-shifted the integer by 1 bit, hence we must add 1 to the scaling. */
    ex = (sint32)((uint64)((x >> 52) & (fpr)0x7FFu));
    ey = (sint32)((uint64)((y >> 52) & (fpr)0x7FFu));
    e = ex - ey - 55 + (sint32)w;

    /* Sign is the XOR of the signs of the operands. */
    s = (sint32)((uint64)((x ^ y) >> 63));

    /* Corrective actions for zeros: if x = 0, then the computation is wrong, and we must clamp e and q to 0. We do not
     * care about the case y = 0 (as per assumptions in this module, the caller does not perform divisions by zero). */
    d = (sint32)((uint32)((uint32)((sint32)(ex + (sint32)0x7FFu)) >> 11));
    s = (sint32)((uint32)((uint32)s & (uint32)d));
    e = (sint32)((uint32)((uint32)e & (uint32)((sint32)((-1) * d))));
    q &= (uint64)((sint32)((-1) * d));

    /* fpr_check_exponent() packs the result and applies proper rounding. */
    return fpr_check_exponent(s, e, q);
}

/***********************************************************************************************************************
* Name:        fpr_scaled
*
* Description: To convert from sint32 to float, we have to do the following:
*              1. Get the absolute value of the input, and its sign
*              2. Shift right or left the value as appropriate
*              3. Pack the result
*
*             We can assume that the source integer is not -2^63.
*
* Arguments:   - sint64 x:  t.b.d.
*              - sint32 sc: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
static fpr fpr_scaled(sint64 i, sint32 sc)
{
    sint32 s = 0;
    sint32 e = 0;
    uint32 t = 0;
    uint64 m = 0;

    /* i_temp is used to avoid modifying the input. */
    sint64 i_temp = i;

    /* Extract sign bit. We have: -i = 1 + ~i */
    s = (sint32) ((uint32)((uint64)i_temp >> 63));
    i_temp = (sint64)((uint64)((uint64)i_temp ^ (uint64)(sint64)((sint32)((-1) * s))));
    i_temp += s;

    /* For now we suppose that i != 0. Otherwise, we set m to i and left-shift it as much as needed to get a 1 in the
     * top bit. We can do that in a logarithmic number of conditional shifts. */
    m = (uint64) i_temp;
    e = 9 + sc;
    fpr_norm64(&m, &e);

    /* Now m is in the 2^63..2^64-1 range. We must divide it by 512; if one of the dropped bits is a 1, this should go
     * into the "sticky bit". */
    m |= (m & 0x1FFu) + 0x1FFu;
    m >>= 9;

    /* Corrective action: if i = 0 then all of the above was incorrect, and we clamp e and m down to zero. */
    t = (uint32) ((uint64)((uint64)(((uint64)i_temp | (uint64)((sint64)((-1) * i_temp)))) >> 63));
    m &= (uint64)((sint64)((-1) * (sint64)t));
    e = (sint32)((uint32)((uint32)e & (uint32)((sint32)((-1) * (sint32) t))));

    /* Assemble back everything. The fpr_check_exponent() function will handle cases where e is too low. */
    return fpr_check_exponent(s, e, m);
}


/***********************************************************************************************************************
* Name:        dddd
*
* Description: Normalize a provided unsigned integer to the 2^63..2^64-1 range by left-shifting it if necessary. The
*              exponent e is adjusted accordingly (i.e. if the value was left-shifted by n bits, then n is subtracted
*              from e). If source m is 0, then it remains 0, but e is altered. Both m and e must be simple variables
*              (no expressions allowed).
*
* Arguments:   - uint64 x:  t.b.d.
*              - sint32 e: t.b.d.
*
* Returns t.b.d.????
*
***********************************************************************************************************************/
static void fpr_norm64(uint64 *mp, sint32 *ep)
{
    uint32 nt = 0;

    uint64 m = *mp;
    sint32 e = *ep;

    e -= 63;

    nt = (uint32)(m >> 32);
    nt = (nt | ((uint32)((sint32)((-1) * (sint32)nt)))) >> 31;
    m ^= (m ^ (m << 32)) & ((uint64)nt - 1u);
    e += (sint32)((uint32)(nt << 5));

    nt = (uint32)(m >> 48);
    nt = (nt | ((uint32)((sint32)((-1) * (sint32)nt)))) >> 31;
    m ^= (m ^ (m << 16)) & ((uint64)nt - 1u);
    e += (sint32)((uint32)(nt << 4));

    nt = (uint32)(m >> 56);
    nt = (nt | ((uint32)((sint32)((-1) * (sint32)nt)))) >> 31;
    m ^= (m ^ (m <<  8)) & ((uint64)nt - 1u);
    e += (sint32)((uint32)(nt << 3));

    nt = (uint32)(m >> 60);
    nt = (nt | ((uint32)((sint32)((-1) * (sint32)nt)))) >> 31;
    m ^= (m ^ (m <<  4)) & ((uint64)nt - 1u);
    e += (sint32)((uint32)(nt << 2));

    nt = (uint32)(m >> 62);
    nt = (nt | ((uint32)((sint32)((-1) * (sint32)nt)))) >> 31;
    m ^= (m ^ (m <<  2)) & ((uint64)nt - 1u);
    e += (sint32)((uint32)(nt << 1));

    nt = (uint32)(m >> 63);
    m ^= (m ^ (m <<  1)) & ((uint64)nt - 1u);
    e += (sint32)nt;

    *mp = m;
    *ep = e;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_of
*
* Description: t.b.d.
*
* Arguments:   - sint64 x:  t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_of(sint64 i)
{
    return fpr_scaled(i, 0);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_rint
*
* Description: t.b.d.
*
* Arguments:   - fpr x:  t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
sint64 FsmSw_Falcon_fpr_rint(fpr x)
{
    uint64 m = 0;
    uint64 d = 0;
    sint32 e = 0;
    uint32 s = 0;
    uint32 dd = 0;
    uint32 f = 0;

    /* We assume that the value fits in -(2^63-1)..+(2^63-1). We can thus extract the mantissa as a 63-bit integer,
     * then right-shift it as needed. */
    m = ((x << 10) | ((uint64)1 << 62)) & (((uint64)1 << 63) - 1u);
    e = (sint32)((uint64)(1085u - ((x >> 52) & (fpr)0x7FFu)));

    /* If a shift of more than 63 bits is needed, then simply set m to zero. This also covers the case of an input
     * operand equal to zero. */
    m &= (uint64)((sint32)((-1) * (sint32)((uint32)(((uint32)e - 64u) >> 31))));
    e = (sint32)((uint32)((uint32)e & 63u));

    /* Right-shift m as needed. Shift count is e. Proper rounding mandates that:
     *   - If the highest dropped bit is zero, then round low.
     *   - If the highest dropped bit is one, and at least one of the other dropped bits is one, then round up.
     *   - If the highest dropped bit is one, and all other dropped bits are zero, then round up if the lowest kept bit
     *     is 1, or low otherwise (i.e. ties are broken by "rounding to even").
     *
     * We thus first extract a word consisting of all the dropped bit AND the lowest kept bit; then we shrink it down
     * to three bits, the lowest being "sticky". */
    d = fpr_ulsh(m, 63 - e);
    dd = (uint32)d | ((uint32)(d >> 32) & 0x1FFFFFFFu);
    f = (uint32)(d >> 61) | ((dd | (uint32)((sint32)((-1) * (sint32)dd))) >> 31);
    m = fpr_ursh(m, e) + (uint64)((uint32)(((uint32)0xC8u >> f) & 1u));

    /* Apply the sign bit. */
    s = (uint32)(x >> 63);
    return (sint64)((uint64)((m ^ (uint64)((uint32)((sint32)((-1) * (sint32)s)))) + s));
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_floor
*
* Description: t.b.d.
*
* Arguments:   - fpr x:  t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
sint64 FsmSw_Falcon_fpr_floor(fpr x)
{
    uint64 t     = 0;
    sint64 xi    = 0;
    sint32 e     = 0;
    sint32 cc    = 0;
    sint64 temp1 = 0;
    uint64 temp2 = 0;

    /* We extract the integer as a _signed_ 64-bit integer with a scaling factor. Since we assume that the value fits
     * in the -(2^63-1)..+(2^63-1) range, we can left-shift the absolute value to make it in the 2^62..2^63-1 range:
     * we will only need a right-shift afterwards. */
    e = (sint32)((uint64)((x >> (fpr)52u) & 0x7FFu));
    t = x >> 63;
    xi = (sint64)((uint64)(((x << 10) | ((uint64)1 << 62)) & (((uint64)1 << 63) - 1u)));
    xi = (sint64)((uint64)((uint64)((uint64)xi ^ (uint64)((sint64)((-1) * (sint64)t))) + t));
    cc = 1085 - e;

    /* We perform an arithmetic right-shift on the value. This applies floor() semantics on both positive and negative
     * values (rounding toward minus infinity). */
    xi = fpr_irsh(xi, (sint32)((uint32)((uint32)cc & 63u)));

    /* If the true shift count was 64 or more, then we should instead replace xi with 0 (if nonnegative) or -1
     * (if negative). Edge case: -0 will be floored to -1, not 0 (whether this is correct is debatable; in any case,
     * the other functions normalize zero to +0).
     * For an input of zero, the non-shifted xi was incorrect (we used a top implicit bit of value 1, not 0), but this
     * does not matter since this operation will clamp it down. */
    temp1 = ((-1) * (sint64)((uint32)((uint32)((uint32)(63u - (uint32)cc)) >> 31)));
    temp2 = (uint64)xi ^ (uint64)((sint64)((-1) * (sint64)t));
    xi = (sint64)((uint64)((uint64)xi ^ (temp2 & (uint64)temp1)));

    return xi;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_trunc
*
* Description: t.b.d.
*
* Arguments:   - fpr x:  t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
sint64 FsmSw_Falcon_fpr_trunc(fpr x)
{
    uint64 t = 0;
    uint64 xu = 0;
    sint32 e = 0;
    sint32 cc = 0;

    /* Extract the absolute value. Since we assume that the value fits in the -(2^63-1)..+(2^63-1) range, we can
     * left-shift the absolute value into the 2^62..2^63-1 range, and then do a right shift afterwards. */
    e = (sint32)((uint64)((x >> (fpr)52) & 0x7FFu));
    xu = ((x << 10) | ((uint64)1 << 62)) & (((uint64)1 << 63) - 1u);
    cc = 1085 - e;
    xu = fpr_ursh(xu, (sint32)((uint32)((uint32)cc & 63u)));

    /* If the exponent is too low (cc > 63), then the shift was wrong and we must clamp the value to 0. This also
     * covers the case of an input equal to zero. */
    xu &= (uint64)((sint64)((-1) * (sint64)((uint32)(((uint32)((uint32)((sint32)(cc - (sint32)64u)))) >> 31))));

    /* Apply back the sign, if the source value is negative. */
    t = x >> 63;
    xu = (uint64)(xu ^ (uint64)((sint64)((-1) * (sint64)t))) + t;

    return (sint64)xu;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_add
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*              - fpr y: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_add(fpr x, fpr y)
{
    uint64 m = 0;
    uint64 xu = 0;
    uint64 yu = 0;
    uint64 za = 0;
    uint32 cs = 0;
    sint32 ex = 0;
    sint32 ey = 0;
    sint32 sx = 0;
    sint32 sy = 0;
    sint32 cc = 0;

    /* x_temp and y_temp are used to avoid modifying the input. */
    fpr x_temp = x;
    fpr y_temp = y;

    /* Make sure that the first operand (x) has the larger absolute value. This guarantees that the exponent of y is
     * less than or equal to the exponent of x, and, if they are equal, then the mantissa of y will not be greater than
     * the mantissa of x.
     * After this swap, the result will have the sign x, except in the following edge case: abs(x) = abs(y), and x and
     * y have opposite sign bits; in that case, the result shall be +0 even if the sign bit of x is 1. To handle this
     * case properly, we do the swap is abs(x) = abs(y) AND the sign of x is 1. */
    m  = ((uint64) 1 << 63) - 1u;
    za = (x_temp & m) - (y_temp & m);
    cs = (uint32) (za >> 63) | ((1u - (uint32) ((uint64)((sint64)((-1) * (sint64)za)) >> 63)) & (uint32) (x_temp >> 63));
    m  = (uint64)((fpr)((x_temp ^ y_temp) & (fpr)((sint32)((-1) * (sint32)cs))));
    x_temp ^= m;
    y_temp ^= m;

    /* Extract sign bits, exponents and mantissas. The mantissas are scaled up to 2^55..2^56-1, and the exponent is
     * unbiased. If an operand is zero, its mantissa is set to 0 at this step, and its exponent will be -1078. */
    ex = (sint32)((uint32)(x_temp >> 52));
    sx = (sint32)((uint32)((uint32)ex >> 11));
    ex = (sint32)((uint32)((uint32)ex & 0x7FFu));
    m  = (uint64)(((uint64)((uint32)((((uint32)((uint32)ex + 0x7FFu)) >> 11))) << 52u));
    xu = ((x_temp & (((uint64) 1 << 52) - 1u)) | m) << 3;

    ex -= (sint32)1078u;
    ey = (sint32)((fpr)(y_temp >> 52));
    sy = (sint32)((uint32)((uint32)ey >> 11));
    ey = (sint32)((uint32)((uint32)ey & 0x7FFu));
    m  = (uint64) (uint32) (((uint32)ey + 0x7FFu) >> 11) << 52;
    yu = ((y_temp & (((uint64) 1 << 52) - 1u)) | m) << 3;
    ey -= (sint32)1078u;

    /* x has the larger exponent; hence, we only need to right-shift y. If the shift count is larger than 59 bits then
     * we clamp the value to zero. */
    cc  = ex - ey;
    yu &= (uint64)((sint64)((-1) * (sint64)((uint32)(((uint32)((sint32)(cc - (sint32)60u))) >> 31))));
    cc  = (sint32)((uint32)((uint32)cc & 63u));

    /* The lowest bit of yu is "sticky". */
    m  = fpr_ulsh(1, cc) - 1u;
    yu |= (yu & m) + m;
    yu = fpr_ursh(yu, cc);

    /* If the operands have the same sign, then we add the mantissas; otherwise, we subtract the mantissas. */
    xu += yu - ((yu << 1) & (uint64)((sint64)((-1) * (sint64)((uint32)((uint32)sx ^ (uint32)sy)))));

    /* The result may be smaller, or slightly larger. We normalize it to the 2^63..2^64-1 range (if xu is zero, then it
     * stays at zero). */
    fpr_norm64(&xu, &ex);

    /* Scale down the value to 2^54..s^55-1, handling the last bit as sticky. */
    xu |= (xu & 0x1FFu) + 0x1FFu;
    xu >>= 9;
    ex += (sint32)9u;

    /* In general, the result has the sign of x. However, if the result is exactly zero, then the following situations
     * may be encountered:
     *   x > 0, y = -x   -> result should be +0
     *   x < 0, y = -x   -> result should be +0
     *   x = +0, y = +0  -> result should be +0
     *   x = -0, y = +0  -> result should be +0
     *   x = +0, y = -0  -> result should be +0
     *   x = -0, y = -0  -> result should be -0
     *
     * But at the conditional swap step at the start of the function, we ensured that if abs(x) = abs(y) and the sign
     * of x was 1, then x and y were swapped. Thus, the two following cases cannot actually happen:
     *   x < 0, y = -x
     *   x = -0, y = +0
     * In all other cases, the sign bit of x is conserved, which is what the fpr_check_exponent() function does.
     * The fpr_check_exponent() function also properly clamps values to zero when the exponent is too low, but does not
     * alter the sign in that case. */

    return fpr_check_exponent(sx, ex, xu);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_sub
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*              - fpr y: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_sub(fpr x, fpr y)
{
    /* y_temp is used to avoid modifying the input. */
    fpr y_temp = y;


    y_temp ^= (uint64)1 << 63;
    return FsmSw_Falcon_fpr_add(x, y_temp);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_neg
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*
* Returns x
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_neg(fpr x)
{
    /* x_temp is used to avoid modifying the input. */
    fpr x_temp = x;

    x_temp ^= (uint64)1 << 63;
    return x_temp;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_half
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*
* Returns x
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_half(fpr x)
{
    /* To divide a value by 2, we just have to subtract 1 from its exponent, but we have to take care of zero. */
    uint32 t;

    /* x_temp is used to avoid modifying the input. */
    fpr x_temp = x;

    x_temp -= (uint64)1 << 52;
    t = (uint32)((uint64)(((uint64)((uint64)(x_temp >> 52) & 0x7FFu)) + 1u) >> 11);
    x_temp &= t - (fpr)1u;
    return x_temp;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_double
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*
* Returns x
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_double(fpr x)
{
    /* x_temp is used to avoid modifying the input. */
    fpr x_temp = x;

    /* To double a value, we just increment by one the exponent. We don't care about infinites or NaNs; however, 0 is a
     * special case. */
    x_temp += (fpr)(((uint64)((uint64)(((uint64)((uint64)(x_temp >> 52) & 0x7FFu) + 0x7FFu) >> 11))) << 52);
    return x_temp;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_sqr
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_sqr(fpr x)
{
    return FsmSw_Falcon_fpr_mul(x, x);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_inv
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_inv(fpr x)
{
    return fpr_div(4607182418800017408u, x);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_lt
*
* Description: If both x and y are positive, then a signed comparison yields the proper result:
*                - For positive values, the order is preserved.
*                - The sign bit is at the same place as in integers, so
*                  sign is preserved.
*              Moreover, we can compute [x < y] as sgn(x-y) and the computation of x-y will not overflow.
*              If the signs differ, then sgn(x) gives the proper result.
*              If both x and y are negative, then the order is reversed. Hence [x < y] = sgn(y-x). We must compute this
*              separately from sgn(x-y); simply inverting sgn(x-y) would not handle the edge case x = y properly.
*
* Arguments:   - fpr x: t.b.d.
*              - fpr y: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon_fpr_lt(fpr x, fpr y)
{
    sint32 cc0 = 0;
    sint32 cc1 = 0;
    sint64 sx = 0;
    sint64 sy = 0;
    sint64 temp1 = 0;
    uint64 temp2 = 0;

    sx = (sint64)x;
    sy = (sint64)y;

    /* set sy=0 if signs differ */
    temp1  = (sint64)((uint64)((uint64)sx ^ (uint64)sy));

    if(temp1 >= 0)
    {
        /* get the first bit */
        temp2 = (uint64)temp1 >> 63;
    }
    else
    {
        temp2 = ((uint64)temp1 >> 63) | 0xF000000000000000u;
    }

    sy = (sint64)((uint64)((uint64)sy & (~((uint64)temp2))));

    /* Neither subtraction overflows when the signs are the same. */
    cc0 = (sint32)((uint64)((uint64)((uint64)(((uint64)((uint64)sx - (uint64)sy))) >> 63) & 1u));
    cc1 = (sint32)((uint64)((uint64)((uint64)(((uint64)((uint64)sy - (uint64)sx))) >> 63) & 1u));

    return (sint32)((uint32)((uint32)cc0 ^ ((uint32)(((uint32)((uint32)cc0 ^ (uint32)cc1)) & ((x & y) >> 63)))));
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_mul
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*              - fpr y: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_mul(fpr x, fpr y)
{
    uint64 xu = 0;
    uint64 yu = 0;
    uint64 w = 0;
    uint64 zu = 0;
    uint64 zv = 0;
    uint32 x0 = 0;
    uint32 x1 = 0;
    uint32 y0 = 0;
    uint32 y1 = 0;
    uint32 z0 = 0;
    uint32 z1 = 0;
    uint32 z2 = 0;
    sint32 ex = 0;
    sint32 ey = 0;
    sint32 d = 0;
    sint32 e = 0;
    sint32 s = 0;

    /* Extract absolute values as scaled unsigned integers. We don't extract exponents yet. */
    xu = (x & (((uint64) 1 << 52) - 1u)) | ((uint64) 1 << 52);
    yu = (y & (((uint64) 1 << 52) - 1u)) | ((uint64) 1 << 52);

    /* We have two 53-bit integers to multiply; we need to split each into a lower half and a upper half. Moreover, we
     * prefer to have lower halves to be of 25 bits each, for reasons explained later on. */
    x0 =  (uint32)(xu & 0x01FFFFFFu);
    x1 =  (uint32)(xu >> 25);
    y0 =  (uint32)(yu & 0x01FFFFFFu);
    y1 =  (uint32)(yu >> 25);
    w  =  (uint64)(x0 * (uint64) y0);
    z0 =  (uint32)(w & 0x01FFFFFFu);
    z1 =  (uint32)(w >> 25);
    w  =  (uint64)(x0 * (uint64) y1);
    z1 += (uint32)(w & 0x01FFFFFFu);
    z2 =  (uint32)(w >> 25);
    w  =  (uint64)(x1 * (uint64) y0);
    z1 += (uint32)(w & 0x01FFFFFFu);
    z2 += (uint32)(w >> 25);
    zu =  (uint64)(x1 * (uint64) y1);
    z2 += (z1 >> 25);
    z1 &= 0x01FFFFFFu;
    zu += z2;

    /* Since xu and yu are both in the 2^52..2^53-1 range, the product is in the 2^104..2^106-1 range. We first
     * reassemble it and round it into the 2^54..2^56-1 range; the bottom bit is made "sticky". Since the low limbs z0
     * and z1 are 25 bits each, we just take the upper part (zu), and consider z0 and z1 only for purposes of
     * stickiness. (This is the reason why we chose 25-bit limbs above.) */

    uint64 temp1 = (uint64)(((uint64)((uint64)z0 | (uint64)z1)) + (uint64)0x01FFFFFFu);
    zu |= temp1 >> 25;

    /* We normalize zu to the 2^54..s^55-1 range: it could be one bit too large at this point. This is done with a
     * conditional right-shift that takes into account the sticky bit. */
    zv = (zu >> 1) | (zu & 1u);
    w = zu >> 55;
    zu ^= (zu ^ zv) & (uint64)((sint64)((-1) * (sint64)w));

    /* Get the aggregate scaling factor:
     *   - Each exponent is biased by 1023.
     *   - Integral mantissas are scaled by 2^52, hence an extra 52 bias for each exponent.
     *   - However, we right-shifted z by 50 bits, and then by 0 or 1 extra bit (depending on the value of w).
     *
     * In total, we must add the exponents, then subtract 2 * (1023 + 52), then add 50 + w. */
    ex = (sint32)((uint32)((uint64)(((uint64)(x >> 52)) & 0x7FFu)));
    ey = (sint32)((uint32)((uint64)(((uint64)(y >> 52)) & 0x7FFu)));
    e = ex + ey - 2100 + (sint32) w;

    /* Sign bit is the XOR of the operand sign bits. */
    s = (sint32)((uint32)((uint64)((uint64)(x ^ y)) >> 63));

    /* Corrective actions for zeros: if either of the operands is zero, then the computations above were wrong. Test
     * for zero is whether ex or ey is zero. We just have to set the mantissa (zu) to zero, the fpr_check_exponent()
     * function will normalize e. */
    d = (sint32)((uint32)((uint32)(((uint32)(((uint32)((uint32)ex + 0x7FFu)) &
                                             ((uint32)((uint32)ey + 0x7FFu)))) >> 11)));
    zu &= (uint64)((sint32)((-1) * d));

    /* fpr_check_exponent() packs the result and applies proper rounding. */
    return fpr_check_exponent(s, e, zu);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_sqrt
*
* Description: t.b.d.
*
* Arguments:   - fpr x: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
fpr FsmSw_Falcon_fpr_sqrt(fpr x)
{
    uint64 xu = 0;
    uint64 q = 0;
    uint64 s = 0;
    uint64 r = 0;
    sint32 ex = 0;
    sint32 e = 0;
    uint64 t = 0;
    uint64 b = 0;

    /* Extract the mantissa and the exponent. We don't care about the sign: by assumption, the operand is nonnegative.
     * We want the "true" exponent corresponding to a mantissa in the 1..2 range. */
    xu = (x & (((uint64) 1 << 52) - 1u)) | ((uint64) 1 << 52);
    ex = (sint32)((uint64)(((uint64)(x >> 52)) & 0x7FFu));
    e = ex - 1023;

    /* If the exponent is odd, double the mantissa and decrement the exponent. The exponent is then halved to account
     * for the square root. */
    xu += (uint64)((uint64)(xu & (uint64)((sint32)((-1) * ((sint32)((uint32)((uint32)e & 1u)))))));
    e = (sint32)((uint32)((uint32)e >> 1));

    /* Double the mantissa. */
    xu <<= 1;

    /* We now have a mantissa in the 2^53..2^55-1 range. It represents a value between 1 (inclusive) and 4 (exclusive)
     * in fixed point notation (with 53 fractional bits). We compute the square root bit by bit. */
    q = 0;
    s = 0;
    r = (uint64) 1 << 53;
    for (sint32 i = 0; i < 54; i++)
    {
        t = s + r;
        b = ((xu - t) >> 63) - 1u;
        s += (r << 1) & b;
        xu -= t & b;
        q += r & b;
        xu <<= 1;
        r >>= 1;
    }

    /* Now, q is a rounded-low 54-bit value, with a leading 1, 52 fractional digits, and an additional guard bit.
     * We add an extra sticky bit to account for what remains of the operand. */
    q <<= 1;
    q |= (xu | (uint64)((sint32)((-1) *  (sint32)xu))) >> 63;

    /* Result q is in the 2^54..2^55-1 range; we bias the exponent by 54 bits (the value e at that point contains the
     * "true" exponent, but q is now considered an integer, i.e. scaled up. */
    e -= 54;

    /* Corrective action for an operand of value zero. */
    q &= (uint64)((sint32)((-1) * (sint32)((uint32)((uint32)((uint32)((uint32)ex + 0x7FFu)) >> 11))));

    /* Apply rounding and back result. */
    return fpr_check_exponent(0, e, q);
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_fpr_expm_p63
*
* Description: Compute exp(x) for x such that |x| <= ln 2. We want a precision of 50 bits or so.
*
* Arguments:   - fpr x:   t.b.d.
*              - fpr ccs: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
uint64 FsmSw_Falcon_fpr_expm_p63(fpr x, fpr ccs)
{
    /* polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" */
    /* Polynomial approximation of exp(-x) is taken from FACCT: https://eprint.iacr.org/2018/1234 */
    /* Specifically, values are extracted from the implementation referenced from the FACCT article, and available at: */
    /* polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" */
    /* https://github.com/raykzhao/gaussian */
    /* Here, the coefficients have been scaled up by 2^63 and converted to integers.
     *
     * Tests over more than 24 billions of random inputs in the 0..log(2) range have never shown a deviation larger
     * than 2^(-50) from the true mathematical value.*/ 
    uint64 z = 0;
    uint64 y = 0;
    uint32 u = 0;
    uint32 z0 = 0;
    uint32 z1 = 0;
    uint32 y0 = 0;
    uint32 y1 = 0;
    uint64 a = 0;
    uint64 b = 0;

    y = C[0];
    z = (uint64) FsmSw_Falcon_fpr_trunc(FsmSw_Falcon_fpr_mul(x, fpr_ptwo63)) << 1;
    for (u = 1; u < (sizeof(C)) / sizeof(C[0]); u++)
    {
        /* Compute product z * y over 128 bits, but keep only the top 64 bits.
         *
         * TODO: On some architectures/compilers we could use some intrinsics (__umulh() on MSVC) or other compiler
         * extensions (unsigned __int128 on GCC / Clang) for improved speed; however, most 64-bit architectures
         * also have appropriate IEEE754 floating-point support, which is better. */
        uint64 c;

        z0 = (uint32) z;
        z1 = (uint32) (z >> 32);
        y0 = (uint32) y;
        y1 = (uint32) (y >> 32);
        a = ((uint64) z0 * (uint64) y1) + (((uint64) z0 * (uint64) y0) >> 32);
        b = ((uint64) z1 * (uint64) y0);
        c = (a >> 32) + (b >> 32);
        c += (((uint64) (uint32) a + (uint64) (uint32) b) >> 32);
        c += (uint64) z1 * (uint64) y1;
        y = C[u] - c;
    }

    /* The scaling factor must be applied at the end. Since y is now in fixed-point notation, we have to convert the
     * factor to the same format, and do an extra integer multiplication. */
    z = (uint64) FsmSw_Falcon_fpr_trunc(FsmSw_Falcon_fpr_mul(ccs, fpr_ptwo63)) << 1;
    z0 = (uint32) z;
    z1 = (uint32) (z >> 32);
    y0 = (uint32) y;
    y1 = (uint32) (y >> 32);
    a = ((uint64) z0 * (uint64) y1) + (((uint64) z0 * (uint64) y0) >> 32);
    b = ((uint64) z1 * (uint64) y0);
    y = (a >> 32) + (b >> 32);
    y += (((uint64) (uint32) a + (uint64) (uint32) b) >> 32);
    y += (uint64) z1 * (uint64) y1;

    return y;
}


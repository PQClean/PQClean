//* Based on the public domain implementation in
//* crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
//* by Ronny Van Keer
//* and the public domain "TweetFips202" implementation
//* from https://twitter.com/tweetfips202
//* by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe
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
 **********************************************************************************************************************/

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Fips202.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define NROUNDS 24u

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
/* Keccak round constants */
static const uint64 KeccakF_RoundConstants[NROUNDS] =
{
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static uint64 load64(const uint8 *x);
static void   store64(uint8 *x, uint64 u);
static void   KeccakF1600_StatePermute(uint64 *state);
static void   keccak_absorb(uint64 *s, uint32 r, const uint8 *m, uint32 mlen, uint8 p);
static void   keccak_inc_finalize(uint64 *s_inc, uint32 r, uint8 p);
static void   keccak_inc_squeeze(uint8 *h, uint32 outlen, uint64 *s_inc, uint32 r);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/***********************************************************************************************************************
* Name:        load64
*
* Description: Load 8 bytes into uint64 in little-endian order
*
* Arguments:   - const uint8 *x: pointer to input byte array
*
* Returns the loaded 64-bit unsigned integer
***********************************************************************************************************************/
static uint64 load64(const uint8 *x)
{
    uint64 r = 0;

    for (uint32 i = 0; i < 8u; ++i)
    {
        r |= (uint64)x[i] << 8u * i;
    }

    return r;
}

/***********************************************************************************************************************
* Name:        store64
*
* Description: Store a 64-bit integer to a byte array in little-endian order
*
* Arguments:   - uint8 *x: pointer to the output byte array
*              - uint64 u: input 64-bit unsigned integer
***********************************************************************************************************************/
static void store64(uint8 *x, uint64 u)
{
    for (uint32 i = 0; i < 8u; ++i)
    {
        x[i] = (uint8) (u >> 8u * i);
    }
}

/***********************************************************************************************************************
* Name:        KeccakF1600_StatePermute
*
* Description: The Keccak F1600 Permutation
*
* Arguments:   - uint64 *state: pointer to input/output Keccak state
***********************************************************************************************************************/
static void KeccakF1600_StatePermute(uint64 *state)
{
    uint8 round;

    uint64 Aba, Abe, Abi, Abo, Abu;
    uint64 Aga, Age, Agi, Ago, Agu;
    uint64 Aka, Ake, Aki, Ako, Aku;
    uint64 Ama, Ame, Ami, Amo, Amu;
    uint64 Asa, Ase, Asi, Aso, Asu;
    uint64 BCa, BCe, BCi, BCo, BCu;
    uint64 Da, De, Di, Do, Du;
    uint64 Eba, Ebe, Ebi, Ebo, Ebu;
    uint64 Ega, Ege, Egi, Ego, Egu;
    uint64 Eka, Eke, Eki, Eko, Eku;
    uint64 Ema, Eme, Emi, Emo, Emu;
    uint64 Esa, Ese, Esi, Eso, Esu;

    // copyFromState(A, state)
    Aba = state[0];
    Abe = state[1];
    Abi = state[2];
    Abo = state[3];
    Abu = state[4];
    Aga = state[5];
    Age = state[6];
    Agi = state[7];
    Ago = state[8];
    Agu = state[9];
    Aka = state[10];
    Ake = state[11];
    Aki = state[12];
    Ako = state[13];
    Aku = state[14];
    Ama = state[15];
    Ame = state[16];
    Ami = state[17];
    Amo = state[18];
    Amu = state[19];
    Asa = state[20];
    Ase = state[21];
    Asi = state[22];
    Aso = state[23];
    Asu = state[24];

    for (round = 0; round < NROUNDS; round = round + 2u) {
        //    prepareTheta
        BCa = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
        BCe = Abe ^ Age ^ Ake ^ Ame ^ Ase;
        BCi = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
        BCo = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
        BCu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;

        // thetaRhoPiChiIotaPrepareTheta(round  , A, E)
        Da = BCu ^ ROL(BCe, 1);
        De = BCa ^ ROL(BCi, 1);
        Di = BCe ^ ROL(BCo, 1);
        Do = BCi ^ ROL(BCu, 1);
        Du = BCo ^ ROL(BCa, 1);

        Aba ^= Da;
        BCa = Aba;
        Age ^= De;
        BCe = ROL(Age, 44);
        Aki ^= Di;
        BCi = ROL(Aki, 43);
        Amo ^= Do;
        BCo = ROL(Amo, 21);
        Asu ^= Du;
        BCu = ROL(Asu, 14);
        Eba = BCa ^ ((~BCe) & BCi);
        Eba ^= KeccakF_RoundConstants[round];
        Ebe = BCe ^ ((~BCi) & BCo);
        Ebi = BCi ^ ((~BCo) & BCu);
        Ebo = BCo ^ ((~BCu) & BCa);
        Ebu = BCu ^ ((~BCa) & BCe);

        Abo ^= Do;
        BCa = ROL(Abo, 28);
        Agu ^= Du;
        BCe = ROL(Agu, 20);
        Aka ^= Da;
        BCi = ROL(Aka, 3);
        Ame ^= De;
        BCo = ROL(Ame, 45);
        Asi ^= Di;
        BCu = ROL(Asi, 61);
        Ega = BCa ^ ((~BCe) & BCi);
        Ege = BCe ^ ((~BCi) & BCo);
        Egi = BCi ^ ((~BCo) & BCu);
        Ego = BCo ^ ((~BCu) & BCa);
        Egu = BCu ^ ((~BCa) & BCe);

        Abe ^= De;
        BCa = ROL(Abe, 1);
        Agi ^= Di;
        BCe = ROL(Agi, 6);
        Ako ^= Do;
        BCi = ROL(Ako, 25);
        Amu ^= Du;
        BCo = ROL(Amu, 8);
        Asa ^= Da;
        BCu = ROL(Asa, 18);
        Eka = BCa ^ ((~BCe) & BCi);
        Eke = BCe ^ ((~BCi) & BCo);
        Eki = BCi ^ ((~BCo) & BCu);
        Eko = BCo ^ ((~BCu) & BCa);
        Eku = BCu ^ ((~BCa) & BCe);

        Abu ^= Du;
        BCa = ROL(Abu, 27);
        Aga ^= Da;
        BCe = ROL(Aga, 36);
        Ake ^= De;
        BCi = ROL(Ake, 10);
        Ami ^= Di;
        BCo = ROL(Ami, 15);
        Aso ^= Do;
        BCu = ROL(Aso, 56);
        Ema = BCa ^ ((~BCe) & BCi);
        Eme = BCe ^ ((~BCi) & BCo);
        Emi = BCi ^ ((~BCo) & BCu);
        Emo = BCo ^ ((~BCu) & BCa);
        Emu = BCu ^ ((~BCa) & BCe);

        Abi ^= Di;
        BCa = ROL(Abi, 62);
        Ago ^= Do;
        BCe = ROL(Ago, 55);
        Aku ^= Du;
        BCi = ROL(Aku, 39);
        Ama ^= Da;
        BCo = ROL(Ama, 41);
        Ase ^= De;
        BCu = ROL(Ase, 2);
        Esa = BCa ^ ((~BCe) & BCi);
        Ese = BCe ^ ((~BCi) & BCo);
        Esi = BCi ^ ((~BCo) & BCu);
        Eso = BCo ^ ((~BCu) & BCa);
        Esu = BCu ^ ((~BCa) & BCe);

        //    prepareTheta
        BCa = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
        BCe = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
        BCi = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
        BCo = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
        BCu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;

        // thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
        Da = BCu ^ ROL(BCe, 1);
        De = BCa ^ ROL(BCi, 1);
        Di = BCe ^ ROL(BCo, 1);
        Do = BCi ^ ROL(BCu, 1);
        Du = BCo ^ ROL(BCa, 1);

        Eba ^= Da;
        BCa = Eba;
        Ege ^= De;
        BCe = ROL(Ege, 44);
        Eki ^= Di;
        BCi = ROL(Eki, 43);
        Emo ^= Do;
        BCo = ROL(Emo, 21);
        Esu ^= Du;
        BCu = ROL(Esu, 14);
        Aba = BCa ^ ((~BCe) & BCi);
        Aba ^= KeccakF_RoundConstants[round + 1u];
        Abe = BCe ^ ((~BCi) & BCo);
        Abi = BCi ^ ((~BCo) & BCu);
        Abo = BCo ^ ((~BCu) & BCa);
        Abu = BCu ^ ((~BCa) & BCe);

        Ebo ^= Do;
        BCa = ROL(Ebo, 28);
        Egu ^= Du;
        BCe = ROL(Egu, 20);
        Eka ^= Da;
        BCi = ROL(Eka, 3);
        Eme ^= De;
        BCo = ROL(Eme, 45);
        Esi ^= Di;
        BCu = ROL(Esi, 61);
        Aga = BCa ^ ((~BCe) & BCi);
        Age = BCe ^ ((~BCi) & BCo);
        Agi = BCi ^ ((~BCo) & BCu);
        Ago = BCo ^ ((~BCu) & BCa);
        Agu = BCu ^ ((~BCa) & BCe);

        Ebe ^= De;
        BCa = ROL(Ebe, 1);
        Egi ^= Di;
        BCe = ROL(Egi, 6);
        Eko ^= Do;
        BCi = ROL(Eko, 25);
        Emu ^= Du;
        BCo = ROL(Emu, 8);
        Esa ^= Da;
        BCu = ROL(Esa, 18);
        Aka = BCa ^ ((~BCe) & BCi);
        Ake = BCe ^ ((~BCi) & BCo);
        Aki = BCi ^ ((~BCo) & BCu);
        Ako = BCo ^ ((~BCu) & BCa);
        Aku = BCu ^ ((~BCa) & BCe);

        Ebu ^= Du;
        BCa = ROL(Ebu, 27);
        Ega ^= Da;
        BCe = ROL(Ega, 36);
        Eke ^= De;
        BCi = ROL(Eke, 10);
        Emi ^= Di;
        BCo = ROL(Emi, 15);
        Eso ^= Do;
        BCu = ROL(Eso, 56);
        Ama = BCa ^ ((~BCe) & BCi);
        Ame = BCe ^ ((~BCi) & BCo);
        Ami = BCi ^ ((~BCo) & BCu);
        Amo = BCo ^ ((~BCu) & BCa);
        Amu = BCu ^ ((~BCa) & BCe);

        Ebi ^= Di;
        BCa = ROL(Ebi, 62);
        Ego ^= Do;
        BCe = ROL(Ego, 55);
        Eku ^= Du;
        BCi = ROL(Eku, 39);
        Ema ^= Da;
        BCo = ROL(Ema, 41);
        Ese ^= De;
        BCu = ROL(Ese, 2);
        Asa = BCa ^ ((~BCe) & BCi);
        Ase = BCe ^ ((~BCi) & BCo);
        Asi = BCi ^ ((~BCo) & BCu);
        Aso = BCo ^ ((~BCu) & BCa);
        Asu = BCu ^ ((~BCa) & BCe);
    }

    // copyToState(state, A)
    state[0] = Aba;
    state[1] = Abe;
    state[2] = Abi;
    state[3] = Abo;
    state[4] = Abu;
    state[5] = Aga;
    state[6] = Age;
    state[7] = Agi;
    state[8] = Ago;
    state[9] = Agu;
    state[10] = Aka;
    state[11] = Ake;
    state[12] = Aki;
    state[13] = Ako;
    state[14] = Aku;
    state[15] = Ama;
    state[16] = Ame;
    state[17] = Ami;
    state[18] = Amo;
    state[19] = Amu;
    state[20] = Asa;
    state[21] = Ase;
    state[22] = Asi;
    state[23] = Aso;
    state[24] = Asu;
}

/***********************************************************************************************************************
* Name:        keccak_absorb
*
* Description: Absorb step of Keccak;
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   -       uint64 *s:    pointer to (uninitialized) output Keccak state
*              -       uint32  r:    rate in bytes (e.g., 168 for SHAKE128)
*              - const uint8  *m:    pointer to input to be absorbed into s
*              -       uint32  mlen: length of input in bytes
*              -       uint8   p:    domain-separation byte for different Keccak-derived functions
***********************************************************************************************************************/
static void keccak_absorb(uint64 *s, uint32 r, const uint8 *m, uint32 mlen, uint8 p)
{
    uint32 i;
    uint8 t[200];

    /* Zero state */
    for (i = 0; i < 25u; ++i)
    {
        s[i] = 0;
    }

    while (mlen >= r)
    {
        for (i = 0; i < r / 8u; ++i)
        {
            s[i] ^= load64(&m[8u * i]);
        }

        KeccakF1600_StatePermute(s);
        mlen -= r;
        m     = &m[r];
    }

    for (i = 0; i < r; ++i)
    {
        t[i] = 0;
    }

    for (i = 0; i < mlen; ++i)
    {
        t[i] = m[i];
    }

    t[i] = p;
    t[r - 1u] |= 128u;

    for (i = 0; i < r / 8u; ++i)
    {
        s[i] ^= load64(&t[8u * i]);
    }
}

/***********************************************************************************************************************
* Name:        keccak_squeezeblocks
*
* Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
*              Modifies the state. Can be called multiple times to keep
*              squeezing, i.e., is incremental.
*
* Arguments:   - uint8  *h:       pointer to output blocks
*              - uint32  nblocks: number of blocks to be squeezed (written to h)
*              - uint64 *s:       pointer to input/output Keccak state
*              - uint32  r:       rate in bytes (e.g., 168 for SHAKE128)
***********************************************************************************************************************/
static void keccak_squeezeblocks(uint8 *h, uint32 nblocks, uint64 *s, uint32 r)
{
    while (nblocks > 0u)
    {
        KeccakF1600_StatePermute(s);

        for (uint32 i = 0; i < (r >> 3); i++)
        {
            store64(&h[8u * i], s[i]);
        }

        h = &h[r];
        nblocks--;
    }
}

/***********************************************************************************************************************
* Name:        keccak_inc_init
*
* Description: Initializes the incremental Keccak state to zero.
*
* Arguments:   - uint64 *s_inc: pointer to input/output incremental state
*                               First 25 values represent Keccak state.
*                               26th value represents either the number of absorbed bytes
*                               that have not been permuted, or not-yet-squeezed bytes.
***********************************************************************************************************************/
static void keccak_inc_init(uint64 *s_inc)
{
    uint32 i;

    for (i = 0; i < 25u; ++i)
    {
        s_inc[i] = 0;
    }

    s_inc[25] = 0;
}

/***********************************************************************************************************************
* Name:        keccak_inc_absorb
*
* Description: Incremental keccak absorb
*              Preceded by keccak_inc_init, succeeded by keccak_inc_finalize
*
* Arguments:   -       uint64 *s_inc: pointer to input/output incremental state
*                                     First 25 values represent Keccak state.
*                                     26th value represents either the number of absorbed bytes
*                                     that have not been permuted, or not-yet-squeezed bytes.
*              -       uint32  r:     rate in bytes (e.g., 168 for SHAKE128)
*              - const uint8  *m:     pointer to input to be absorbed into s
*              -       uint32  mlen:  length of input in bytes
***********************************************************************************************************************/
static void keccak_inc_absorb(uint64 *s_inc, uint32 r, const uint8 *m, uint32 mlen)
{
    uint32 i;

    /* Recall that s_inc[25] is the non-absorbed bytes xored into the state */
    while (mlen + s_inc[25] >= r)
    {
        for (i = 0; i < r - (uint32)s_inc[25]; i++)
        {
            /* Take the i'th byte from message
               xor with the s_inc[25] + i'th byte of the state; little-endian */
            s_inc[(s_inc[25] + i) >> 3] ^= (uint64)m[i] << (8u * ((s_inc[25] + i) & 0x07u));
        }

        mlen -= (uint32)(r - s_inc[25]);
        m = &m[r - s_inc[25]];
        s_inc[25] = 0;

        KeccakF1600_StatePermute(s_inc);
    }

    for (i = 0; i < mlen; i++)
    {
        s_inc[(s_inc[25] + i) >> 3] ^= (uint64)m[i] << (8u * ((s_inc[25] + i) & 0x07u));
    }

    s_inc[25] += mlen;
}

/***********************************************************************************************************************
* Name:        keccak_inc_finalize
*
* Description: Finalizes Keccak absorb phase, prepares for squeezing
*
* Arguments:   - uint64 *s_inc: pointer to input/output incremental state
*                               First 25 values represent Keccak state.
*                               26th value represents either the number of absorbed bytes
*                               that have not been permuted, or not-yet-squeezed bytes.
*              - uint32  r:     rate in bytes (e.g., 168 for SHAKE128)
*              - uint8   p:     domain-separation byte for different Keccak-derived functions
***********************************************************************************************************************/
static void keccak_inc_finalize(uint64 *s_inc, uint32 r, uint8 p)
{
    /* After keccak_inc_absorb, we are guaranteed that s_inc[25] < r,
       so we can always use one more byte for p in the current state. */
    s_inc[s_inc[25] >> 3] ^= (uint64)p << (8u * (s_inc[25] & 0x07u));
    s_inc[(r - 1u) >> 3] ^= (uint64)128u << (8u * ((r - 1u) & 0x07u));
    s_inc[25] = 0;
}

/***********************************************************************************************************************
* Name:        keccak_inc_squeeze
*
* Description: Incremental Keccak squeeze; can be called on byte-level
*
* Arguments:   - uint8  *h:      pointer to output bytes
*              - uint32  outlen: number of bytes to be squeezed
*              - uint64 *s_inc:  pointer to input/output incremental state
*                                First 25 values represent Keccak state.
*                                26th value represents either the number of absorbed bytes
*                                that have not been permuted, or not-yet-squeezed bytes.
*              - uint32  r:      rate in bytes (e.g., 168 for SHAKE128)
***********************************************************************************************************************/
static void keccak_inc_squeeze(uint8 *h, uint32 outlen, uint64 *s_inc, uint32 r)
{
    uint32 i;

    /* First consume any bytes we still have sitting around */
    for (i = 0; i < outlen && i < s_inc[25]; i++) {
        /* There are s_inc[25] bytes left, so r - s_inc[25] is the first
           available byte. We consume from there, i.e., up to r. */
        h[i] = (uint8)(s_inc[(r - s_inc[25] + i) >> 3] >> (8u * ((r - s_inc[25] + i) & 0x07u)));
    }

    h = &h[i];
    outlen -= i;
    s_inc[25] -= i;

    /* Then squeeze the remaining necessary blocks */
    while (outlen > 0u)
    {
        KeccakF1600_StatePermute(s_inc);

        for (i = 0; i < outlen && i < r; i++)
        {
            h[i] = (uint8)(s_inc[i >> 3] >> (8u * (i & 0x07u)));
        }

        h = &h[i];
        outlen -= i;
        s_inc[25] = (uint64)r - i;
    }
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

void FsmSw_Fips202_shake128_inc_init(shake128incctx *state)
{
    keccak_inc_init(state->ctx);
}

void FsmSw_Fips202_shake128_inc_absorb(shake128incctx *state, const uint8 *input, uint32 inlen)
{
    keccak_inc_absorb(state->ctx, SHAKE128_RATE, input, inlen);
}

void FsmSw_Fips202_shake128_inc_finalize(shake128incctx *state)
{
    keccak_inc_finalize(state->ctx, SHAKE128_RATE, 0x1F);
}

void FsmSw_Fips202_shake128_inc_squeeze(uint8 *output, uint32 outlen, shake128incctx *state)
{
    keccak_inc_squeeze(output, outlen, state->ctx, SHAKE128_RATE);
}

void FsmSw_Fips202_shake128_inc_ctx_clone(shake128incctx *dest, const shake128incctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
}

void FsmSw_Fips202_shake256_inc_init(shake256incctx *state)
{
    keccak_inc_init(state->ctx);
}

void FsmSw_Fips202_shake256_inc_absorb(shake256incctx *state, const uint8 *input, uint32 inlen)
{
    keccak_inc_absorb(state->ctx, SHAKE256_RATE, input, inlen);
}

void FsmSw_Fips202_shake256_inc_finalize(shake256incctx *state)
{
    keccak_inc_finalize(state->ctx, SHAKE256_RATE, 0x1F);
}

void FsmSw_Fips202_shake256_inc_squeeze(uint8 *output, uint32 outlen, shake256incctx *state)
{
    keccak_inc_squeeze(output, outlen, state->ctx, SHAKE256_RATE);
}

void FsmSw_Fips202_shake256_inc_ctx_clone(shake256incctx *dest, const shake256incctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake128_absorb
*
* Description: Absorb step of the SHAKE128 XOF.
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   -       uint64 *s:     pointer to (uninitialized) output Keccak state
*              - const uint8  *input: pointer to input to be absorbed into s
*              -       uint32  inlen: length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_shake128_absorb(shake128ctx *state, const uint8 *input, uint32 inlen)
{
    keccak_absorb(state->ctx, SHAKE128_RATE, input, inlen, 0x1F);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake128_squeezeblocks
*
* Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of
*              SHAKE128_RATE bytes each. Modifies the state. Can be called
*              multiple times to keep squeezing, i.e., is incremental.
*
* Arguments:   - uint8       *output:  pointer to output blocks
*              - uint32       nblocks: number of blocks to be squeezed (written to output)
*              - shake128ctx *state:   pointer to input/output Keccak state
***********************************************************************************************************************/
void FsmSw_Fips202_shake128_squeezeblocks(uint8 *output, uint32 nblocks, shake128ctx *state)
{
    keccak_squeezeblocks(output, nblocks, state->ctx, SHAKE128_RATE);
}

void FsmSw_Fips202_shake128_ctx_clone(shake128ctx *dest, const shake128ctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKECTX_BYTES);
}


/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake256_absorb
*
* Description: Absorb step of the SHAKE256 XOF.
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   -       shake256ctx *state: pointer to (uninitialized) output Keccak state
*              - const uint8       *input: pointer to input to be absorbed into s
*              -       uint32       inlen: length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_shake256_absorb(shake256ctx *state, const uint8 *input, uint32 inlen)
{
    keccak_absorb(state->ctx, SHAKE256_RATE, input, inlen, 0x1F);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake256_squeezeblocks
*
* Description: Squeeze step of SHAKE256 XOF. Squeezes full blocks of
*              SHAKE256_RATE bytes each. Modifies the state. Can be called
*              multiple times to keep squeezing, i.e., is incremental.
*
* Arguments:   - uint8       *output:  pointer to output blocks
*              - uint32       nblocks: number of blocks to be squeezed (written to output)
*              - shake256ctx *state:   pointer to input/output Keccak state
***********************************************************************************************************************/
void FsmSw_Fips202_shake256_squeezeblocks(uint8 *output, uint32 nblocks, shake256ctx *state)
{
    keccak_squeezeblocks(output, nblocks, state->ctx, SHAKE256_RATE);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake256_ctx_clone
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_shake256_ctx_clone(shake256ctx *dest, const shake256ctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKECTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake128
*
* Description: SHAKE128 XOF with non-incremental API
*
* Arguments:   -       uint8  *output: pointer to output
*              -       uint32  outlen: requested output length in bytes
*              - const uint8  *input:  pointer to input
*              -       uint32  inlen:  length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_shake128(uint8 *output, uint32 outlen, const uint8 *input, uint32 inlen)
{
    uint32 nblocks = outlen / SHAKE128_RATE;
    uint8 t[SHAKE128_RATE];
    shake128ctx s;

    FsmSw_Fips202_shake128_absorb(&s, input, inlen);
    FsmSw_Fips202_shake128_squeezeblocks(output, nblocks, &s);

    output = &output[nblocks * SHAKE128_RATE];
    outlen -= nblocks * SHAKE128_RATE;

    if (outlen > 0u)
    {
        FsmSw_Fips202_shake128_squeezeblocks(t, 1, &s);
        for (uint32 i = 0; i < outlen; ++i)
        {
            output[i] = t[i];
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_shake256
*
* Description: SHAKE256 XOF with non-incremental API
*
* Arguments:   -       uint8  *output: pointer to output
*              -       uint32  outlen: requested output length in bytes
*              - const uint8  *input:  pointer to input
*              -       uint32  inlen:  length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_shake256(uint8 *output, uint32 outlen, const uint8 *input, uint32 inlen)
{
    uint32 nblocks = outlen / SHAKE256_RATE;
    uint8 t[SHAKE256_RATE];
    shake256ctx s;

    FsmSw_Fips202_shake256_absorb(&s, input, inlen);
    FsmSw_Fips202_shake256_squeezeblocks(output, nblocks, &s);

    output = &output[nblocks * SHAKE256_RATE];
    outlen -= nblocks * SHAKE256_RATE;

    if (outlen > 0u)
    {
        FsmSw_Fips202_shake256_squeezeblocks(t, 1, &s);

        for (uint32 i = 0; i < outlen; ++i)
        {
            output[i] = t[i];
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_256_inc_init
*
* Description: ????? TODO: Create description
*
* Arguments:   -       sha3_256incctx *state:  pointer to ????? TODO: Create description
*
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_256_inc_init(sha3_256incctx *state)
{
    keccak_inc_init(state->ctx);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_256_inc_ctx_clone
*
* Description: ????? TODO: Create description
*
* Arguments:   -       sha3_256incctx *dest:  pointer to ????? TODO: Create description
*              - const sha3_256incctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_256_inc_ctx_clone(sha3_256incctx *dest, const sha3_256incctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_256_inc_absorb
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_256_inc_absorb(sha3_256incctx *state, const uint8 *input, uint32 inlen)
{
    keccak_inc_absorb(state->ctx, SHA3_256_RATE, input, inlen);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_256_inc_finalize
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_256_inc_finalize(uint8 *output, sha3_256incctx *state)
{
    uint8 t[SHA3_256_RATE];

    keccak_inc_finalize(state->ctx, SHA3_256_RATE, 0x06);

    keccak_squeezeblocks(t, 1, state->ctx, SHA3_256_RATE);

    for (uint32 i = 0; i < 32u; i++)
    {
        output[i] = t[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_256
*
* Description: SHA3-256 with non-incremental API
*
* Arguments:   - uint8 *output:      pointer to output
*              - const uint8 *input: pointer to input
*              - uint32 inlen:       length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_256(uint8 *output, const uint8 *input, uint32 inlen)
{
    uint64 s[25];
    uint8 t[SHA3_256_RATE];

    /* Absorb input */
    keccak_absorb(s, SHA3_256_RATE, input, inlen, 0x06);

    /* Squeeze output */
    keccak_squeezeblocks(t, 1, s, SHA3_256_RATE);

    for (uint32 i = 0; i < 32u; i++)
    {
        output[i] = t[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_384_inc_init
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_384_inc_init(sha3_384incctx *state)
{
    keccak_inc_init(state->ctx);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_384_inc_ctx_clone
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_384_inc_ctx_clone(sha3_384incctx *dest, const sha3_384incctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_384_inc_absorb
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_384_inc_absorb(sha3_384incctx *state, const uint8 *input, uint32 inlen)
{
    keccak_inc_absorb(state->ctx, SHA3_384_RATE, input, inlen);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_384_inc_finalize
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_384_inc_finalize(uint8 *output, sha3_384incctx *state)
{
    uint8 t[SHA3_384_RATE];
    keccak_inc_finalize(state->ctx, SHA3_384_RATE, 0x06);

    keccak_squeezeblocks(t, 1, state->ctx, SHA3_384_RATE);

    for (uint32 i = 0; i < 48u; i++)
    {
        output[i] = t[i];
    }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Fips202_sha3_384
 *
 * Description: SHA3-256 with non-incremental API
 *
 * Arguments:   - uint8 *output:      pointer to output
 *              - const uint8 *input: pointer to input
 *              - uint32 inlen:   length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_384(uint8 *output, const uint8 *input, uint32 inlen)
{
    uint64 s[25];
    uint8 t[SHA3_384_RATE];

    /* Absorb input */
    keccak_absorb(s, SHA3_384_RATE, input, inlen, 0x06);

    /* Squeeze output */
    keccak_squeezeblocks(t, 1u, s, SHA3_384_RATE);

    for (uint32 i = 0; i < 48u; i++)
    {
        output[i] = t[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_512_inc_init
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_512_inc_init(sha3_512incctx *state)
{
    keccak_inc_init(state->ctx);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_512_inc_ctx_clone
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_512_inc_ctx_clone(sha3_512incctx *dest, const sha3_512incctx *src)
{
    FsmSw_CommonLib_memcpy(dest->ctx, src->ctx, PQC_SHAKEINCCTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_512_inc_absorb
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_512_inc_absorb(sha3_512incctx *state, const uint8 *input, uint32 inlen)
{
    keccak_inc_absorb(state->ctx, SHA3_512_RATE, input, inlen);
}

/***********************************************************************************************************************
* Name:        FsmSw_Fips202_sha3_512_inc_finalize
*
* Description: ????? TODO: Create description
*
* Arguments:   -       shake256ctx *dest:  pointer to ????? TODO: Create description
*              - const shake256ctx *src:   pointer to ????? TODO: Create description
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_512_inc_finalize(uint8 *output, sha3_512incctx *state)
{
    uint8 t[SHA3_512_RATE];
    keccak_inc_finalize(state->ctx, SHA3_512_RATE, 0x06);

    keccak_squeezeblocks(t, 1, state->ctx, SHA3_512_RATE);

    for (uint32 i = 0; i < 64u; i++)
    {
        output[i] = t[i];
    }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Fips202_sha3_512
 *
 * Description: SHA3-512 with non-incremental API
 *
 * Arguments:   - uint8 *output:      pointer to output
 *              - const uint8 *input: pointer to input
 *              - uint32 inlen:   length of input in bytes
***********************************************************************************************************************/
void FsmSw_Fips202_sha3_512(uint8 *output, const uint8 *input, uint32 inlen)
{
    uint64 s[25];
    uint8 t[SHA3_512_RATE];

    /* Absorb input */
    keccak_absorb(s, SHA3_512_RATE, input, inlen, 0x06);

    /* Squeeze output */
    keccak_squeezeblocks(t, 1, s, SHA3_512_RATE);

    for (uint32 i = 0; i < 64u; i++)
    {
        output[i] = t[i];
    }
}

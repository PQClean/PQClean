#include "align.h"
#include "indcpa.h"
#include "kem.h"
#include "params.h"
#include "randombytes.h"
#include "symmetric.h"
#include "verify.h"
#include <stddef.h>
#include <stdint.h>


/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - unsigned char *pk: pointer to output public key
*                (an already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key
*                (an already allocated array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_KYBER512_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    size_t i = 0;
    PQCLEAN_KYBER512_AVX2_indcpa_keypair(pk, sk);
    for (i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
    }
    hash_h(sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    /* Value z for pseudo-random output on reject */
    randombytes(sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES);
    return 0;
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - unsigned char *ct: pointer to output cipher text
*                (an already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
*              - unsigned char *ss: pointer to output shared secret
*                (an already allocated array of CRYPTO_BYTES bytes)
*              - const unsigned char *pk: pointer to input public key
*                (an already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_KYBER512_AVX2_crypto_kem_enc(unsigned char *ct,
        unsigned char *ss,
        const unsigned char *pk) {
    ALIGN32_ARRAY(uint8_t, 2 * KYBER_SYMBYTES) buf;
    /* Will contain key, coins */
    ALIGN32_ARRAY(uint8_t, 2 * KYBER_SYMBYTES) kr;

    randombytes(buf.arr, KYBER_SYMBYTES);
    /* Don't release system RNG output */
    hash_h(buf.arr, buf.arr, KYBER_SYMBYTES);

    /* Multitarget countermeasure for coins + contributory KEM */
    hash_h(buf.arr + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    hash_g(kr.arr, buf.arr, 2 * KYBER_SYMBYTES);

    /* coins are in kr+KYBER_SYMBYTES */
    PQCLEAN_KYBER512_AVX2_indcpa_enc(ct, buf.arr, pk, kr.arr + KYBER_SYMBYTES);

    /* overwrite coins in kr with H(c) */
    hash_h(kr.arr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);
    /* hash concatenation of pre-k and H(c) to k */
    kdf(ss, kr.arr, 2 * KYBER_SYMBYTES);
    return 0;
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - unsigned char *ss: pointer to output shared secret
*                (an already allocated array of CRYPTO_BYTES bytes)
*              - const unsigned char *ct: pointer to input cipher text
*                (an already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
*              - const unsigned char *sk: pointer to input private key
*                (an already allocated array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
**************************************************/
int PQCLEAN_KYBER512_AVX2_crypto_kem_dec(unsigned char *ss,
        const unsigned char *ct,
        const unsigned char *sk) {
    size_t i = 0;
    int fail = 0;
    ALIGN32_ARRAY(uint8_t, 2 * KYBER_SYMBYTES) buf;
    /* Will contain key, coins */
    ALIGN32_ARRAY(uint8_t, 2 * KYBER_SYMBYTES) kr;
    uint8_t cmp[KYBER_CIPHERTEXTBYTES];
    const uint8_t *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;

    PQCLEAN_KYBER512_AVX2_indcpa_dec(buf.arr, ct, sk);

    /* Multitarget countermeasure for coins + contributory KEM */
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        buf.arr[KYBER_SYMBYTES + i] = sk[KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES + i];
    }
    hash_g(kr.arr, buf.arr, 2 * KYBER_SYMBYTES);

    /* coins are in kr+KYBER_SYMBYTES */
    PQCLEAN_KYBER512_AVX2_indcpa_enc(cmp, buf.arr, pk, kr.arr + KYBER_SYMBYTES);

    fail = PQCLEAN_KYBER512_AVX2_verify(ct, cmp, KYBER_CIPHERTEXTBYTES);

    /* overwrite coins in kr with H(c) */
    hash_h(kr.arr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);

    /* Overwrite pre-k with z on re-encryption failure */
    PQCLEAN_KYBER512_AVX2_cmov(kr.arr, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES, fail);

    /* hash concatenation of pre-k and H(c) to k */
    kdf(ss, kr.arr, 2 * KYBER_SYMBYTES);
    return 0;
}

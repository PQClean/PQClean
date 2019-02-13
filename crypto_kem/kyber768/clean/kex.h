#ifndef KEX_H
#define KEX_H

#include "api.h"
#include "params.h"

#define KYBER_UAKE_SENDABYTES (KYBER_PUBLICKEYBYTES + KYBER_CIPHERTEXTBYTES)
#define KYBER_UAKE_SENDBBYTES (KYBER_CIPHERTEXTBYTES)

#define KYBER_AKE_SENDABYTES (KYBER_PUBLICKEYBYTES + KYBER_CIPHERTEXTBYTES)
#define KYBER_AKE_SENDBBYTES (2 * KYBER_CIPHERTEXTBYTES)

typedef unsigned char u8;

void PQCLEAN_KYBER768_kyber_uake_initA(u8 *send, u8 *tk, u8 *sk, const u8 *pkb);

void PQCLEAN_KYBER768_kyber_uake_sharedB(u8 *send, u8 *k, const u8 *recv,
        const u8 *skb);

void PQCLEAN_KYBER768_kyber_uake_sharedA(u8 *k, const u8 *recv, const u8 *tk,
        const u8 *sk);

void PQCLEAN_KYBER768_kyber_ake_initA(u8 *send, u8 *tk, u8 *sk, const u8 *pkb);

void PQCLEAN_KYBER768_kyber_ake_sharedB(u8 *send, u8 *k, const u8 *recv,
                                        const u8 *skb, const u8 *pka);

void PQCLEAN_KYBER768_kyber_ake_sharedA(u8 *k, const u8 *recv, const u8 *tk,
                                        const u8 *sk, const u8 *ska);

#endif

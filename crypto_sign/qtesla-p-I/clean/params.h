#ifndef PQCLEAN_QTESLAPI_CLEAN_PARAMS_H
#define PQCLEAN_QTESLAPI_CLEAN_PARAMS_H

/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: provably-secure qTESLA parameters
**************************************************************************************/

#define PARAM_N 1024
#define PARAM_N_LOG 10
#define PARAM_SIGMA 8.5
#define PARAM_Q 343576577
#define PARAM_Q_LOG 29
#define PARAM_QINV 2205847551
#define PARAM_BARR_MULT 3
#define PARAM_BARR_DIV 30
#define PARAM_B 524287
#define PARAM_B_BITS 19
#define PARAM_S_BITS 8
#define PARAM_K 4
#define PARAM_SIGMA_E PARAM_SIGMA
#define PARAM_H 25
#define PARAM_D 22
#define PARAM_GEN_A 108
#define PARAM_KEYGEN_BOUND_E 554
#define PARAM_E PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 554
#define PARAM_S PARAM_KEYGEN_BOUND_S
#define PARAM_R2_INVN 13632409
#define PARAM_R 172048372

#define SHAKE shake128
#define cSHAKE cshake128
#define SHAKE_RATE SHAKE128_RATE

#define CRYPTO_RANDOMBYTES 32
#define CRYPTO_SEEDBYTES 32
#define CRYPTO_C_BYTES 32
#define HM_BYTES 64

#endif

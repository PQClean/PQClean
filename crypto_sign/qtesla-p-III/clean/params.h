#ifndef PQCLEAN_QTESLAPIII_CLEAN_PARAMS_H
#define PQCLEAN_QTESLAPIII_CLEAN_PARAMS_H

/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: provably-secure qTESLA parameters
**************************************************************************************/

#define PARAM_N 2048
#define PARAM_N_LOG 11
#define PARAM_SIGMA 8.5
#define PARAM_Q 856145921
#define PARAM_Q_LOG 30
#define PARAM_QINV 587710463
#define PARAM_BARR_MULT 5
#define PARAM_BARR_DIV 32
#define PARAM_B 2097151
#define PARAM_B_BITS 21
#define PARAM_S_BITS 8
#define PARAM_K 5
#define PARAM_SIGMA_E PARAM_SIGMA
#define PARAM_H 40
#define PARAM_D 24
#define PARAM_GEN_A 180
#define PARAM_KEYGEN_BOUND_E 901
#define PARAM_E PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 901
#define PARAM_S PARAM_KEYGEN_BOUND_S
#define PARAM_R2_INVN 513161157
#define PARAM_R 14237691

#define SHAKE shake256
#define cSHAKE cshake256
#define SHAKE_RATE SHAKE256_RATE

#define CRYPTO_RANDOMBYTES 32
#define CRYPTO_SEEDBYTES 32
#define CRYPTO_C_BYTES 32
#define HM_BYTES 64

#endif

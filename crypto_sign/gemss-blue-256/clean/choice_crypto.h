#ifndef _CHOICE_PQCLEAN_GEMSSBLUE256_CLEAN_CRYPTO_H
#define _CHOICE_PQCLEAN_GEMSSBLUE256_CLEAN_CRYPTO_H


/* To test the library. */
/* Value 0: the user can modify parameters to test the library. */
/* The other values are the number of 32-bit words of an element of GF(2^n),
   i.e. ceil(n/32), between 1 and 18. */
/* #define TEST_LIBRARY (0) */



/** Level of security of the cryptosystem and the hash functions. */
#define K 256U



/** Choice of the current cryptosystem. */
#define BlueGeMSS
/* To choose parameters of GeMSS */
/* #define GeMSS */
/* To choose parameters of BlueGeMSS */
/* #define BlueGeMSS */
/* To choose parameters of RedGeMSS */
/* #define RedGeMSS */
/* To choose parameters of FGeMSS */
/* #define FGeMSS */
/* To choose parameters of DualModeMS */
/* #define DualModeMS */
/* To use QUARTZ (version 2, is broken) */
/* #define QUARTZ */
/* To use QUARTZ_V1 (is broken) */
/* #define QUARTZ_V1 */


#define INNER_DualModeMS 1




/** Choice of a HFE-based multivariate scheme. */
#define HFE 1



/** Boolean for the inner mode. */
#define InnerMode 1
/** Boolean for the dual mode. */
#define DualMode 0

/** Boolean enabling the code of the dual mode. */
#define ENABLED_DUALMODE 0




/** Prefix which is the name of the current used cryptosystem. */
#define PQCLEAN_GEMSSBLUE256_CLEAN_CRYPTO_NAME BlueGeMSS_



/** Set the EUF-CMA property of the cryptosystem. */
#define EUF_CMA_PROPERTY 0





#endif


#ifndef _SIZES_PQCLEAN_GEMSSBLUE192_AVX2_CRYPTO_H
#define _SIZES_PQCLEAN_GEMSSBLUE192_AVX2_CRYPTO_H
#include "choice_crypto.h"
#include "sizes_HFE.h"



/************************ SIZES ************************/

/* Theoretical sizes in bits */
/* Practical sizes in bytes */


/* Sizes with the inner mode */
/** Theoretical in bits size of the public-key (inner mode). */
#define SIZE_PK_THEORETICAL_INNER SIZE_PK_THEORETICAL_HFE
/** Theoretical in bits size of the secret-key (inner mode). */
#define SIZE_SK_THEORETICAL_INNER SIZE_SK_THEORETICAL_HFE
/** Theoretical in bits size of the signature (inner mode). */
#define SIZE_SIGN_THEORETICAL_INNER SIZE_SIGN_THEORETICAL_HFE

/** Practical size in bytes of the public-key (inner mode). */
#define SIZE_PK_INNER SIZE_PK_HFE
/** Practical size in bytes of the secret-key (inner mode). */
#define SIZE_SK_INNER SIZE_SK_HFE
/** Practical size in bytes of the signature (inner mode). */
#define SIZE_SIGN_INNER SIZE_SIGN_HFE


/** Theoretical size in bits of the public-key. */
#define SIZE_PK_THEORETICAL SIZE_PK_THEORETICAL_INNER
/** Theoretical size in bits of the secret-key. */
#define SIZE_SK_THEORETICAL SIZE_SK_THEORETICAL_INNER
/** Theoretical size in bits of the signature. */
#define SIZE_SIGN_THEORETICAL SIZE_SIGN_THEORETICAL_INNER

/** Practical size in bytes of the public-key. */
#define SIZE_PK SIZE_PK_INNER
/** Practical size in bytes of the secret-key. */
#define SIZE_SK SIZE_SK_INNER
/** Practical size in bytes of the signature. */
#define SIZE_SIGN SIZE_SIGN_INNER




#endif

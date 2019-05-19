#pragma once
// CATEGORY defined in the makefile

/*----------------------------------------------------------------------------*/
#if CATEGORY == 1
#define TRNG_BYTE_LENGTH (24)
#define    HASH_FUNCTION sha3_256
#define HASH_BYTE_LENGTH (32)
// N0 defined in the makefile
#if (DFR_SL_LEVEL == 0)
#define    P  (35899)  // modulus(x) = x^P-1
#define    DV (9)  // odd number
#define    M  (9)
#define    M0 (5)
#define    M1 (4)
#define    NUM_ERRORS_T   (136)

#elif (DFR_SL_LEVEL == 1)
#define    P  (52147)  // modulus(x) = x^P-1
#define    DV (9)  // odd number
#define    M  (9)
#define    M0 (5)
#define    M1 (4)
#define    NUM_ERRORS_T   (136)

#else
#error "Unsupported number of circulant blocks"
#endif
#endif // end CATEGORY == 1

/*----------------------------------------------------------------------------*/

// We employ the parameters for Category 3 also in the case where the required
// security level is Category 2, where Category 2 has the following parameters.
//   #define TRNG_BYTE_LENGTH (32)
//   #define    HASH_FUNCTION sha3_256
//   #define HASH_BYTE_LENGTH (32)

/*----------------------------------------------------------------------------*/

#if (CATEGORY == 2) || (CATEGORY == 3)
#define TRNG_BYTE_LENGTH (32)
#define    HASH_FUNCTION sha3_384
#define HASH_BYTE_LENGTH (48)
// N0 defined in the makefile
#if (DFR_SL_LEVEL == 0)
#define    P  (57899)  // modulus(x) = x^P-1
#define    DV (11)  // odd number
#define    M  (11)
#define    M0 (6)
#define    M1 (5)
#define    NUM_ERRORS_T   (199)

#elif (DFR_SL_LEVEL == 1)
#define    P  (96221)  // modulus(x) = x^P-1
#define    DV (11)  // odd number
#define    M  (11)
#define    M0 (6)
#define    M1 (5)
#define    NUM_ERRORS_T   (199)

#else
#error "Unsupported number of circulant blocks"
#endif
#endif

/*----------------------------------------------------------------------------*/

// We employ the parameters for Category 4 also in the case where the required
// security level is Category 5, where Category 4 has the following parameters.
// #if CATEGORY == 4
//   #define TRNG_BYTE_LENGTH (40)
//   #define    HASH_FUNCTION sha3_384
//   #define HASH_BYTE_LENGTH (48)
// #endif

/*----------------------------------------------------------------------------*/

#if (CATEGORY == 4) || (CATEGORY == 5)
#define TRNG_BYTE_LENGTH (40)
#define    HASH_FUNCTION  sha3_512
#define HASH_BYTE_LENGTH (64)
// N0 defined in the makefile
#if (DFR_SL_LEVEL == 0)
#define    P  (89051)  // modulus(x) = x^P-1
#define    DV (13)  // odd number
#define    M  (13)
#define    M0 (7)
#define    M1 (6)
#define    NUM_ERRORS_T   (267)

#elif (DFR_SL_LEVEL == 1)
#define    P  (152267)  // modulus(x) = x^P-1
#define    DV (13)  // odd number
#define    M  (13)
#define    M0 (7)
#define    M1 (6)
#define    NUM_ERRORS_T   (267)

#else
#error "Unsupported number of circulant blocks"
#endif
#endif
/*----------------------------------------------------------------------------*/

// Derived parameters, they are useful for QC-LDPC algorithms
#define HASH_BIT_LENGTH (HASH_BYTE_LENGTH << 3)
#define               K ((N0-1)*P)
#define               N (N0*P)
#define              DC (N0*DV)

// Circulant weight structure of the Q matrix, specialized per value of N0
#if N0 == 2
#define    Q_BLOCK_WEIGHTS  {{M0,M1},{M1,M0}}
#elif N0 == 3
#define    Q_BLOCK_WEIGHTS  {{M0,M1,M2},{M2,M0,M1},{M1,M2,M0}}
#elif N0 == 4
#define    Q_BLOCK_WEIGHTS  {{M0,M1,M2,M3},{M3,M0,M1,M2},{M2,M3,M0,M1},{M1,M2,M3,M0}}
#else
#error "Unsupported number of circulant blocks"
#endif

static const unsigned char qBlockWeights[N0][N0] = Q_BLOCK_WEIGHTS;


/*----------------------------------------------------------------------------*/

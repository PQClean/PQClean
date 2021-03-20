#ifndef QC_LDPC_PARAMETERS
#define QC_LDPC_PARAMETERS

//TODO: insert here the right values
#define    N0  (2)
#define    CATEGORY (1)
#define DFR_SL_LEVEL (0)

#define    P  (23371)
#define    V  (71)
#define    NUM_ERRORS_T  (130)


#define TRNG_BYTE_LENGTH (24)
#define    HASH_FUNCTION sha3_256
#define HASH_BYTE_LENGTH (32)


// Derived parameters, they are useful for QC-LDPC algorithms
#define HASH_BIT_LENGTH (HASH_BYTE_LENGTH << 3)
#define               K ((N0-1)*P)
#define               N (N0*P)

#endif
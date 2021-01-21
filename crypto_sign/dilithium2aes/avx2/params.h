#ifndef PQCLEAN_DILITHIUM2AES_AVX2_PARAMS_H
#define PQCLEAN_DILITHIUM2AES_AVX2_PARAMS_H



#define SEEDBYTES 32
#define CRHBYTES 48
#define N 256
#define Q 8380417
#define D 13
#define ROOT_OF_UNITY 1753

#define K 4
#define L 4
#define ETA 2
#define TAU 39
#define BETA 78
#define GAMMA1 (1 << 17)
#define GAMMA2 ((Q-1)/88)
#define OMEGA 80
#define PQCLEAN_DILITHIUM2AES_AVX2_CRYPTO_ALGNAME "Dilithium2-AES"


#define POLYT1_PACKEDBYTES  320
#define POLYT0_PACKEDBYTES  416
#define POLYVECH_PACKEDBYTES (OMEGA + K)

#define POLYZ_PACKEDBYTES   576

#define POLYW1_PACKEDBYTES  192

#define POLYETA_PACKEDBYTES  96

#define PQCLEAN_DILITHIUM2AES_AVX2_CRYPTO_PUBLICKEYBYTES (SEEDBYTES + K*POLYT1_PACKEDBYTES)
#define PQCLEAN_DILITHIUM2AES_AVX2_CRYPTO_SECRETKEYBYTES (2*SEEDBYTES + CRHBYTES \
        + L*POLYETA_PACKEDBYTES \
        + K*POLYETA_PACKEDBYTES \
        + K*POLYT0_PACKEDBYTES)
#define PQCLEAN_DILITHIUM2AES_AVX2_CRYPTO_BYTES (SEEDBYTES + L*POLYZ_PACKEDBYTES + POLYVECH_PACKEDBYTES)

#endif

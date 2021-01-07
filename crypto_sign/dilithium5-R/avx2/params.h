#ifndef PARAMS_H
#define PARAMS_H

#define SEEDBYTES 32
#define CRHBYTES 48
#define N 256
#define Q 8380417
#define D 13
#define ROOT_OF_UNITY 1753
#define DILITHIUM_NAMESPACE(s) PQCLEAN_DILITHIUM5R_AVX2##s



#define K 8
#define L 7
#define ETA 2
#define TAU 60
#define BETA 120
#define GAMMA1 (1 << 19)
#define GAMMA2 ((Q-1)/32)
#define OMEGA 75


#define POLYT1_PACKEDBYTES  320
#define POLYT0_PACKEDBYTES  416
#define POLYVECH_PACKEDBYTES (OMEGA + K)

#define POLYZ_PACKEDBYTES   640

#define POLYW1_PACKEDBYTES  128

#define POLYETA_PACKEDBYTES  96

#define CRYPTO_PUBLICKEYBYTES (SEEDBYTES + K*POLYT1_PACKEDBYTES)
#define CRYPTO_SECRETKEYBYTES (2*SEEDBYTES + CRHBYTES \
                               + L*POLYETA_PACKEDBYTES \
                               + K*POLYETA_PACKEDBYTES \
                               + K*POLYT0_PACKEDBYTES)
#define CRYPTO_BYTES (SEEDBYTES + L*POLYZ_PACKEDBYTES + POLYVECH_PACKEDBYTES)

#endif

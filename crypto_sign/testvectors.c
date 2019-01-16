#include "api.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 100
#define MAXMLEN 2048

static void printbytes(const unsigned char *x, unsigned long long xlen) {
    unsigned long long i;
    for (i = 0; i < xlen; i++) {
        printf("%02x", x[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char sk[CRYPTO_SECRETKEYBYTES];
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];

    unsigned char mi[MAXMLEN];
    unsigned char sm[MAXMLEN + CRYPTO_BYTES];
    unsigned long long smlen;
    unsigned long long mlen;

    int r;
    unsigned long long i, k;

    for (i = 0; i < MAXMLEN; i = (i == 0) ? i + 1 : i << 1) {
        randombytes(mi, i);

        crypto_sign_keypair(pk, sk);

        printbytes(pk, CRYPTO_PUBLICKEYBYTES);
        printbytes(sk, CRYPTO_SECRETKEYBYTES);

        crypto_sign(sm, &smlen, mi, i, sk);

        printbytes(sm, smlen);

        // By relying on m == sm we prevent having to allocate CRYPTO_BYTES
        // twice
        r = crypto_sign_open(sm, &mlen, sm, smlen, pk);

        if (r) {
            printf("ERROR: signature verification failed\n");
            return -1;
        }
        for (k = 0; k < i; k++) {
            if (sm[k] != mi[k]) {
                printf("ERROR: message recovery failed\n");
                return -1;
            }
        }
    }
    return 0;
}

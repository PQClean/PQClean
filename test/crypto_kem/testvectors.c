#include "api.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 100

static void printbytes(const unsigned char *x, unsigned long long xlen) {
    unsigned long long i;
    for (i = 0; i < xlen; i++) {
        printf("%02x", x[i]);
    }
    printf("\n");
}

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)

#define crypto_kem_keypair NAMESPACE(crypto_kem_keypair)
#define crypto_kem_enc NAMESPACE(crypto_kem_enc)
#define crypto_kem_dec NAMESPACE(crypto_kem_dec)

int main(void) {
    unsigned char key_a[CRYPTO_BYTES], key_b[CRYPTO_BYTES];
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char sendb[CRYPTO_CIPHERTEXTBYTES];
    unsigned char sk_a[CRYPTO_SECRETKEYBYTES];
    int i, j;
    for (i = 0; i < NTESTS; i++) {
        // Key-pair generation
        crypto_kem_keypair(pk, sk_a);

        printbytes(pk, CRYPTO_PUBLICKEYBYTES);
        printbytes(sk_a, CRYPTO_SECRETKEYBYTES);

        // Encapsulation
        crypto_kem_enc(sendb, key_b, pk);

        printbytes(sendb, CRYPTO_CIPHERTEXTBYTES);
        printbytes(key_b, CRYPTO_BYTES);

        // Decapsulation
        crypto_kem_dec(key_a, sendb, sk_a);
        printbytes(key_a, CRYPTO_BYTES);

        for (j = 0; j < CRYPTO_BYTES; j++) {
            if (key_a[j] != key_b[j]) {
                printf("ERROR\n");
                return -1;
            }
        }
    }
    return 0;
}

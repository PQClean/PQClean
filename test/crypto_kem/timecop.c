#include "api.h"
#include "randombytes.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <valgrind/memcheck.h>

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)

#define CRYPTO_BYTES           NAMESPACE(CRYPTO_BYTES)
#define CRYPTO_PUBLICKEYBYTES  NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define CRYPTO_SECRETKEYBYTES  NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define CRYPTO_CIPHERTEXTBYTES NAMESPACE(CRYPTO_CIPHERTEXTBYTES)
#define CRYPTO_ALGNAME NAMESPACE(CRYPTO_ALGNAME)

#define crypto_kem_keypair NAMESPACE(crypto_kem_keypair)
#define crypto_kem_enc NAMESPACE(crypto_kem_enc)
#define crypto_kem_dec NAMESPACE(crypto_kem_dec)

static void poison(void *addr, size_t len) {
    VALGRIND_MAKE_MEM_UNDEFINED(addr, len);
}

static void unpoison(void *addr, size_t len) {
    VALGRIND_MAKE_MEM_DEFINED(addr, len);
}

int main(void) {
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t c[CRYPTO_CIPHERTEXTBYTES];
    uint8_t k[CRYPTO_BYTES];

    int result;

    result = crypto_kem_keypair(pk, sk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_kem_keypair\n");
        return result;
    }

    unpoison(pk, CRYPTO_PUBLICKEYBYTES);
    result = crypto_kem_enc(c, k, pk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_kem_enc\n");
        return result;
    }

    poison(sk, CRYPTO_SECRETKEYBYTES);
    unpoison(c, CRYPTO_CIPHERTEXTBYTES);
    result = crypto_kem_dec(k, c, sk);
    if (result) {
        fprintf(stderr, "Error in crypto_kem_dec\n");
        return result;
    }

    return 0;
}

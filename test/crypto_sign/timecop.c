#include "api.h"
#include "randombytes.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <valgrind/memcheck.h>

#define MLEN 1024

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)

#define CRYPTO_BYTES           NAMESPACE(CRYPTO_BYTES)
#define CRYPTO_PUBLICKEYBYTES  NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define CRYPTO_SECRETKEYBYTES  NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define CRYPTO_ALGNAME NAMESPACE(CRYPTO_ALGNAME)

#define crypto_sign_keypair NAMESPACE(crypto_sign_keypair)
#define crypto_sign NAMESPACE(crypto_sign)
#define crypto_sign_open NAMESPACE(crypto_sign_open)
#define crypto_sign_signature NAMESPACE(crypto_sign_signature)
#define crypto_sign_verify NAMESPACE(crypto_sign_verify)

static void poison(void *addr, size_t len) {
    VALGRIND_MAKE_MEM_UNDEFINED(addr, len);
}

static void unpoison(void *addr, size_t len) {
    VALGRIND_MAKE_MEM_DEFINED(addr, len);
}

static int test_sign(void) {
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t sm[MLEN + CRYPTO_BYTES];
    uint8_t m[MLEN];

    size_t mlen;
    size_t smlen;

    int result;

    randombytes(m, MLEN);

    result = crypto_sign_keypair(pk, sk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_sign_keypair\n");
        return result;
    }

    poison(m, MLEN);
    poison(sk, CRYPTO_SECRETKEYBYTES);
    result = crypto_sign(sm, &smlen, m, MLEN, sk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_sign\n");
        return result;
    }

    unpoison(sm, MLEN + CRYPTO_BYTES);
    unpoison(pk, CRYPTO_PUBLICKEYBYTES);
    result = crypto_sign_open(sm, &mlen, sm, smlen, pk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_sign_open\n");
        return result;
    }

    return 0;
}

static int test_sign_detatched(void) {
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t sig[CRYPTO_BYTES];
    uint8_t m[MLEN];

    size_t siglen;
    int result;

    randombytes(m, MLEN);

    result = crypto_sign_keypair(pk, sk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_sign_keypair\n");
        return result;
    }

    poison(m, CRYPTO_SECRETKEYBYTES);
    poison(sk, CRYPTO_SECRETKEYBYTES);
    result = crypto_sign_signature(sig, &siglen, m, MLEN, sk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_sign_signature\n");
        return result;
    }

    unpoison(&sig, CRYPTO_BYTES);
    unpoison(&m, MLEN);
    unpoison(&pk, CRYPTO_PUBLICKEYBYTES);
    result = crypto_sign_verify(sig, siglen, m, MLEN, pk);
    unpoison(&result, sizeof result);
    if (result) {
        fprintf(stderr, "Error in crypto_sign_verify\n");
        return result;
    }

    return 0;
}

int main(void) {
    int result;

    result = test_sign();
    if (result) {
        fprintf(stderr, "Error in test_sign\n");
        return result;
    }

    result = test_sign_detatched();
    if (result) {
        fprintf(stderr, "Error in test_sign_detatched\n");
        return result;
    }

    return 0;
}

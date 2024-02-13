#include "api.h"
#include "randombytes.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PQCLEAN_USE_VALGRIND
#include <valgrind/memcheck.h>
#endif

#ifdef PQCLEAN_FALCON_TEST_INTEROP
// workaround to include the proper header in both
// pytest and manual testing

// https://stackoverflow.com/a/40063340
#define STR(x) #x
#define STRINGIFY(x) STR(x)
#define EXPAND(x) x
#define CONCAT(x, y) STRINGIFY(EXPAND(x)EXPAND(y))

#include CONCAT(PQCLEAN_INTEROP_INCLUDE,/api.h)
#endif

#ifndef NTESTS
#define NTESTS 5
#endif

#define MLEN 1024

const uint8_t canary[8] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};

/* allocate a bit more for all keys and messages and
 * make sure it is not touched by the implementations.
 */
static void write_canary(uint8_t *d) {
    for (size_t i = 0; i < 8; i++) {
        d[i] = canary[i];
    }
}

static int check_canary(const uint8_t *d) {
    for (size_t i = 0; i < 8; i++) {
        if (d[i] != canary[i]) {
            return -1;
        }
    }
    return 0;
}

/** Safe malloc */
inline static void *malloc_s(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror("Malloc failed!");
        exit(1);
    }
    return ptr;
}

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)

#define CRYPTO_PUBLICKEYBYTES NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define CRYPTO_SECRETKEYBYTES NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define CRYPTO_BYTES          NAMESPACE(CRYPTO_BYTES)
#define CRYPTO_ALGNAME        NAMESPACE(CRYPTO_ALGNAME)

#define crypto_sign_keypair NAMESPACE(crypto_sign_keypair)
#define crypto_sign NAMESPACE(crypto_sign)
#define crypto_sign_open NAMESPACE(crypto_sign_open)
#define crypto_sign_signature NAMESPACE(crypto_sign_signature)
#define crypto_sign_verify NAMESPACE(crypto_sign_verify)

#ifdef PQCLEAN_FALCON_TEST_INTEROP
// workaround since crypto_sign_verify is already tokenized
#define INTEROP_PASTER(x, y, z) x##_##y##_##z
#define INTEROP_EVALUATOR(x, y, z) INTEROP_PASTER(x, y, z)
#define INTEROP_NAMESPACE(y, z) INTEROP_EVALUATOR(PQCLEAN_INTEROP_NAMESPACE, y, z)
#define crypto_sign_verify_interop INTEROP_NAMESPACE(crypto, sign_verify)
#endif

#define RETURNS_ZERO(f)                           \
    if ((f) != 0) {                               \
        puts("(f) returned non-zero returncode"); \
        res = 1;                                  \
        goto end;                                 \
    }

// https://stackoverflow.com/a/55243651/248065
#define MY_TRUTHY_VALUE_X 1
#define CAT(x,y) CAT_(x,y)
#define CAT_(x,y) x##y
#define HAS_NAMESPACE(x) CAT(CAT(MY_TRUTHY_VALUE_,CAT(PQCLEAN_NAMESPACE,CAT(_,x))),X)

#if !HAS_NAMESPACE(API_H)
#error "namespace not properly defined for header guard"
#endif

static int test_sign(void) {
    /*
     * In order to properly test variable-length signatures, we need to check
     * that the implementation does not modify the provided buffer beyond the
     * reported signature length. We do this by filling the buffer with random
     * bytes before the call to sign and checking afterward that the tail has
     * not been modified.
    */
    uint8_t sm_random_cmp[MLEN + CRYPTO_BYTES];

    /*
     * This is most likely going to be aligned by the compiler.
     * 16 extra bytes for canary
     * 1 extra byte for unalignment
     */
    uint8_t *pk_aligned = malloc_s(CRYPTO_PUBLICKEYBYTES + 16 + 1);
    uint8_t *sk_aligned = malloc_s(CRYPTO_SECRETKEYBYTES + 16 + 1);
    uint8_t *sm_aligned = malloc_s(MLEN + CRYPTO_BYTES + 16 + 1);
    uint8_t *m_aligned = malloc_s(MLEN + 16 + 1);

    /*
     * Make sure all pointers are odd.
     * This ensures that the implementation does not assume anything about the
     * data alignment. For example this would catch if an implementation
     * directly uses these pointers to load into vector registers using movdqa.
     */
    uint8_t *pk = (uint8_t *) ((uintptr_t) pk_aligned | (uintptr_t) 1);
    uint8_t *sk = (uint8_t *) ((uintptr_t) sk_aligned | (uintptr_t) 1);
    uint8_t *sm = (uint8_t *) ((uintptr_t) sm_aligned | (uintptr_t) 1);
    uint8_t *m  = (uint8_t *) ((uintptr_t) m_aligned | (uintptr_t) 1);

    size_t mlen;
    size_t smlen;
    int returncode;
    int res = 0;

    int i;
    /*
     * Write 8 byte canary before and after the actual memory regions.
     * This is used to validate that the implementation does not assume
     * anything about the placement of data in memory
     * (e.g., assuming that the pk is always behind the sk)
     */
    write_canary(pk);
    write_canary(pk + CRYPTO_PUBLICKEYBYTES + 8);
    write_canary(sk);
    write_canary(sk + CRYPTO_SECRETKEYBYTES + 8);
    write_canary(sm);
    write_canary(sm + MLEN + CRYPTO_BYTES + 8);
    write_canary(m);
    write_canary(m + MLEN + 8);

    for (i = 0; i < NTESTS; i++) {
        RETURNS_ZERO(crypto_sign_keypair(pk + 8, sk + 8));

        randombytes(m + 8, MLEN);
        // Fill the sm buffer with random bytes
        randombytes(sm_random_cmp, MLEN + CRYPTO_BYTES);
        memcpy(sm + 8, sm_random_cmp, MLEN + CRYPTO_BYTES);

#ifdef PQCLEAN_USE_VALGRIND
        /*
         * With this buffer marked as undefined, valgrind will detect
         * cases where the signing code depends on the value of the tail
         * of the buffer.
         */
        VALGRIND_MAKE_MEM_UNDEFINED(sm + 8, MLEN + CRYPTO_BYTES);
#endif

        RETURNS_ZERO(crypto_sign(sm + 8, &smlen, m + 8, MLEN, sk + 8));

#ifdef PQCLEAN_USE_VALGRIND
        // We have to mark the tail as defined before doing the memcmp.
        VALGRIND_MAKE_MEM_DEFINED(sm + 8 + smlen, MLEN + CRYPTO_BYTES - smlen);
#endif

        // check that the tail has not been modified
        RETURNS_ZERO(memcmp(sm + 8 + smlen, sm_random_cmp + smlen, MLEN + CRYPTO_BYTES - smlen));

        // By relying on m == sm we prevent having to allocate CRYPTO_BYTES
        // twice
        if ((returncode =
                    crypto_sign_open(sm + 8, &mlen, sm + 8, smlen, pk + 8)) != 0) {
            fprintf(stderr, "ERROR Signature did not verify correctly!\n");
            if (returncode > 0) {
                fprintf(stderr, "ERROR return code should be < 0 on failure");
            }
            res = 1;
            goto end;
        }
        // Validate that the implementation did not touch the canary
        if (check_canary(pk) || check_canary(pk + CRYPTO_PUBLICKEYBYTES + 8) ||
                check_canary(sk) || check_canary(sk + CRYPTO_SECRETKEYBYTES + 8) ||
                check_canary(sm) || check_canary(sm + MLEN + CRYPTO_BYTES + 8) ||
                check_canary(m) || check_canary(m + MLEN + 8)) {
            fprintf(stderr, "ERROR canary overwritten\n");
            res = 1;
            goto end;
        }
    }
end:
    free(pk_aligned);
    free(sk_aligned);
    free(sm_aligned);
    free(m_aligned);

    return res;
}

static int test_sign_detached(void) {
    /*
     * In order to properly test variable-length signatures, we need to check
     * that the implementation does not modify the provided buffer beyond the
     * reported signature length. We do this by filling the buffer with random
     * bytes before the call to sign and checking afterward that the tail has
     * not been modified.
    */
    uint8_t sig_random_cmp[CRYPTO_BYTES];

    /*
     * This is most likely going to be aligned by the compiler.
     * 16 extra bytes for canary
     * 1 extra byte for unalignment
     */
    uint8_t *pk_aligned = malloc_s(CRYPTO_PUBLICKEYBYTES + 16 + 1);
    uint8_t *sk_aligned = malloc_s(CRYPTO_SECRETKEYBYTES + 16 + 1);
    uint8_t *sig_aligned = malloc_s(CRYPTO_BYTES + 16 + 1);
    uint8_t *m_aligned = malloc_s(MLEN + 16 + 1);

    /*
     * Make sure all pointers are odd.
     * This ensures that the implementation does not assume anything about the
     * data alignment. For example this would catch if an implementation
     * directly uses these pointers to load into vector registers using movdqa.
     */
    uint8_t *pk = (uint8_t *) ((uintptr_t) pk_aligned | (uintptr_t) 1);
    uint8_t *sk = (uint8_t *) ((uintptr_t) sk_aligned | (uintptr_t) 1);
    uint8_t *sig = (uint8_t *) ((uintptr_t) sig_aligned | (uintptr_t) 1);
    uint8_t *m  = (uint8_t *) ((uintptr_t) m_aligned | (uintptr_t) 1);

    size_t siglen;
    int returncode;
    int res = 0;

    int i;
    /*
     * Write 8 byte canary before and after the actual memory regions.
     * This is used to validate that the implementation does not assume
     * anything about the placement of data in memory
     * (e.g., assuming that the pk is always behind the sk)
     */
    write_canary(pk);
    write_canary(pk + CRYPTO_PUBLICKEYBYTES + 8);
    write_canary(sk);
    write_canary(sk + CRYPTO_SECRETKEYBYTES + 8);
    write_canary(sig);
    write_canary(sig + CRYPTO_BYTES + 8);
    write_canary(m);
    write_canary(m + MLEN + 8);

    for (i = 0; i < NTESTS; i++) {
        RETURNS_ZERO(crypto_sign_keypair(pk + 8, sk + 8));

        randombytes(m + 8, MLEN);

        // Fill the sig buffer with random bytes
        randombytes(sig_random_cmp, CRYPTO_BYTES);
        memcpy(sig + 8, sig_random_cmp, CRYPTO_BYTES);

#ifdef PQCLEAN_USE_VALGRIND
        /*
         * With this buffer marked as undefined, valgrind will detect
         * cases where the signing code depends on the value of the tail
         * of the buffer.
         */
        VALGRIND_MAKE_MEM_UNDEFINED(sig + 8, CRYPTO_BYTES);
#endif

        RETURNS_ZERO(crypto_sign_signature(sig + 8, &siglen, m + 8, MLEN, sk + 8));

#ifdef PQCLEAN_USE_VALGRIND
        // We have to mark the tail as defined before doing the memcmp.
        VALGRIND_MAKE_MEM_DEFINED(sig + 8 + siglen, CRYPTO_BYTES - siglen);
#endif

        // check that the tail has not been modified
        RETURNS_ZERO(memcmp(sig + 8 + siglen, sig_random_cmp + siglen, CRYPTO_BYTES - siglen));

        if ((returncode =
                    crypto_sign_verify(sig + 8, siglen, m + 8, MLEN, pk + 8)) != 0) {
            fprintf(stderr, "ERROR Signature did not verify correctly!\n");
            if (returncode > 0) {
                fprintf(stderr, "ERROR return code should be < 0 on failure");
            }
            res = 1;
            goto end;
        }

#ifdef PQCLEAN_FALCON_TEST_INTEROP
        // test verification with the "-padded" or non "-padded" code as appropriate
        if ((returncode =
                    crypto_sign_verify_interop(sig + 8, siglen, m + 8, MLEN, pk + 8)) != 0) {
            fprintf(stderr, "ERROR Signature did not verify correctly on interop check!\n");
            if (returncode > 0) {
                fprintf(stderr, "ERROR return code should be < 0 on failure");
            }
            res = 1;
            goto end;
        }
#endif

        // Validate that the implementation did not touch the canary
        if (check_canary(pk) || check_canary(pk + CRYPTO_PUBLICKEYBYTES + 8) ||
                check_canary(sk) || check_canary(sk + CRYPTO_SECRETKEYBYTES + 8) ||
                check_canary(sig) || check_canary(sig + CRYPTO_BYTES + 8) ||
                check_canary(m) || check_canary(m + MLEN + 8)) {
            fprintf(stderr, "ERROR canary overwritten\n");
            res = 1;
            goto end;
        }
    }

end:
    free(pk_aligned);
    free(sk_aligned);
    free(sig_aligned);
    free(m_aligned);

    return res;
}

static int test_wrong_pk(void) {
    uint8_t *pk = malloc_s(CRYPTO_PUBLICKEYBYTES);
    uint8_t *pk2 = malloc_s(CRYPTO_PUBLICKEYBYTES);
    uint8_t *sk = malloc_s(CRYPTO_SECRETKEYBYTES);
    uint8_t *sm = malloc_s(MLEN + CRYPTO_BYTES);
    uint8_t *m = malloc_s(MLEN);

    size_t mlen;
    size_t smlen;

    int returncode, res = 0;

    int i;

    for (i = 0; i < NTESTS; i++) {
        RETURNS_ZERO(crypto_sign_keypair(pk2, sk));

        RETURNS_ZERO(crypto_sign_keypair(pk, sk));

        randombytes(m, MLEN);
        RETURNS_ZERO(crypto_sign(sm, &smlen, m, MLEN, sk));

        // By relying on m == sm we prevent having to allocate CRYPTO_BYTES
        // twice
        returncode = crypto_sign_open(sm, &mlen, sm, smlen, pk2);
        if (!returncode) {
            fprintf(stderr, "ERROR Signature did verify correctly under wrong public key!\n");
            if (returncode > 0) {
                fprintf(stderr, "ERROR return code should be < 0");
            }
            res = 1;
            goto end;
        }
    }
end:
    free(pk);
    free(pk2);
    free(sk);
    free(sm);
    free(m);
    return res;
}

int main(void) {
    // check if CRYPTO_ALGNAME is printable
    puts(CRYPTO_ALGNAME);
    int result = 0;
    result += test_sign();
    result += test_sign_detached();
    result += test_wrong_pk();

    return result;
}

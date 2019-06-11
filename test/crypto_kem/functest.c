#include "api.h"
#include "randombytes.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef NTESTS
#define NTESTS 5
#endif

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

// https://stackoverflow.com/a/1489985/1711232
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

#define RETURNS_ZERO(f)                           \
    if ((f) != 0) {                               \
        puts(#f " returned non-zero returncode"); \
        return -1;                                \
    }

// https://stackoverflow.com/a/55243651/248065
#define MY_TRUTHY_VALUE_X 1
#define CAT(x,y) CAT_(x,y)
#define CAT_(x,y) x##y
#define HAS_NAMESPACE(x) CAT(CAT(MY_TRUTHY_VALUE_,CAT(PQCLEAN_NAMESPACE,CAT(_,x))),X)

#if !HAS_NAMESPACE(API_H)
#error "namespace not properly defined for header guard"
#endif

static int test_keys(void) {
    /*
     * This is most likely going to be aligned by the compiler.
     * 16 extra bytes for canary
     * 1 extra byte for unalignment
     */

    uint8_t key_a_aligned[CRYPTO_BYTES + 16 + 1];
    uint8_t key_b_aligned[CRYPTO_BYTES + 16 + 1];
    uint8_t pk_aligned[CRYPTO_PUBLICKEYBYTES + 16 + 1];
    uint8_t sendb_aligned[CRYPTO_CIPHERTEXTBYTES + 16 + 1];
    uint8_t sk_a_aligned[CRYPTO_SECRETKEYBYTES + 16 + 1];

    /*
     * Make sure all pointers are odd.
     * This ensures that the implementation does not assume anything about the
     * data alignment. For example this would catch if an implementation
     * directly uses these pointers to load into vector registers using movdqa.
     */
    uint8_t *key_a = (uint8_t *) ((uintptr_t) key_a_aligned|(uintptr_t) 1);
    uint8_t *key_b = (uint8_t *) ((uintptr_t) key_b_aligned|(uintptr_t) 1);
    uint8_t *pk    = (uint8_t *) ((uintptr_t) pk_aligned|(uintptr_t) 1);
    uint8_t *sendb = (uint8_t *) ((uintptr_t) sendb_aligned|(uintptr_t) 1);
    uint8_t *sk_a  = (uint8_t *) ((uintptr_t) sk_a_aligned|(uintptr_t) 1);

    /*
     * Write 8 byte canary before and after the actual memory regions.
     * This is used to validate that the implementation does not assume
     * anything about the placement of data in memory
     * (e.g., assuming that the pk is always behind the sk)
     */
    write_canary(key_a);
    write_canary(key_a + CRYPTO_BYTES + 8);
    write_canary(key_b);
    write_canary(key_b + CRYPTO_BYTES + 8);
    write_canary(pk);
    write_canary(pk +  CRYPTO_PUBLICKEYBYTES + 8);
    write_canary(sendb);
    write_canary(sendb + CRYPTO_CIPHERTEXTBYTES + 8);
    write_canary(sk_a);
    write_canary(sk_a + CRYPTO_SECRETKEYBYTES + 8);

    int i;

    for (i = 0; i < NTESTS; i++) {
        // Alice generates a public key
        RETURNS_ZERO(crypto_kem_keypair(pk + 8, sk_a + 8));

        // Bob derives a secret key and creates a response
        RETURNS_ZERO(crypto_kem_enc(sendb + 8, key_b + 8, pk + 8));

        // Alice uses Bobs response to get her secret key
        RETURNS_ZERO(crypto_kem_dec(key_a + 8, sendb + 8, sk_a + 8));

        if (memcmp(key_a + 8, key_b + 8, CRYPTO_BYTES) != 0) {
            printf("ERROR KEYS\n");
            return -1;
        }

        // Validate that the implementation did not touch the canary
        if (check_canary(key_a) || check_canary(key_a + CRYPTO_BYTES + 8) ||
            check_canary(key_b) || check_canary(key_b + CRYPTO_BYTES + 8 ) ||
            check_canary(pk) || check_canary(pk + CRYPTO_PUBLICKEYBYTES + 8 ) ||
            check_canary(sendb) || check_canary(sendb + CRYPTO_CIPHERTEXTBYTES + 8 ) ||
            check_canary(sk_a) || check_canary(sk_a + CRYPTO_SECRETKEYBYTES + 8 )) {
            printf("ERROR canary overwritten\n");
            return -1;
        }
    }

    return 0;
}

static int test_invalid_sk_a(void) {
    uint8_t sk_a[CRYPTO_SECRETKEYBYTES];
    uint8_t key_a[CRYPTO_BYTES], key_b[CRYPTO_BYTES];
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sendb[CRYPTO_CIPHERTEXTBYTES];
    int i;
    int returncode;

    for (i = 0; i < NTESTS; i++) {
        // Alice generates a public key
        RETURNS_ZERO(crypto_kem_keypair(pk, sk_a));

        // Bob derives a secret key and creates a response
        RETURNS_ZERO(crypto_kem_enc(sendb, key_b, pk));

        // Replace secret key with random values
        randombytes(sk_a, CRYPTO_SECRETKEYBYTES);

        // Alice uses Bobs response to get her secret key
        if ((returncode = crypto_kem_dec(key_a, sendb, sk_a)) > 0) {
            printf("ERROR failing crypto_kem_dec returned %d instead of "
                   "negative or zero code\n",
                   returncode);
            return -1;
        }

        if (!memcmp(key_a, key_b, CRYPTO_BYTES)) {
            printf("ERROR invalid sk_a\n");
            return -1;
        }
    }

    return 0;
}

static int test_invalid_ciphertext(void) {
    uint8_t sk_a[CRYPTO_SECRETKEYBYTES];
    uint8_t key_a[CRYPTO_BYTES], key_b[CRYPTO_BYTES];
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sendb[CRYPTO_CIPHERTEXTBYTES];
    int i;
    int returncode;

    for (i = 0; i < NTESTS; i++) {
        // Alice generates a public key
        RETURNS_ZERO(crypto_kem_keypair(pk, sk_a));

        // Bob derives a secret key and creates a response
        RETURNS_ZERO(crypto_kem_enc(sendb, key_b, pk));

        // Change ciphertext to random value
        randombytes(sendb, sizeof(sendb));

        // Alice uses Bobs response to get her secret key
        if ((returncode = crypto_kem_dec(key_a, sendb, sk_a)) > 0) {
            printf("ERROR crypto_kem_dec should either fail (negative "
                   "returncode) or succeed (return 0) but returned %d\n",
                   returncode);
            return -1;
        }

        if (!memcmp(key_a, key_b, CRYPTO_BYTES)) {
            printf("ERROR invalid ciphertext\n");
            return -1;
        }
    }

    return 0;
}

int main(void) {
    // Check if CRYPTO_ALGNAME is printable
    puts(CRYPTO_ALGNAME);

    int result = 0;
    result += test_keys();
    result += test_invalid_sk_a();
    result += test_invalid_ciphertext();

    if (result != 0) {
        puts("Errors occurred");
    }
    return result;
}

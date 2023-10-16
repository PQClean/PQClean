#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "api.h"
#include "randombytes.h"

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)

#define CRYPTO_BYTES           NAMESPACE(CRYPTO_BYTES)
#define CRYPTO_PUBLICKEYBYTES  NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define CRYPTO_SECRETKEYBYTES  NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define CRYPTO_CIPHERTEXTBYTES NAMESPACE(CRYPTO_CIPHERTEXTBYTES)
#define CRYPTO_ALGNAME         NAMESPACE(CRYPTO_ALGNAME)

#define crypto_kem_keypair NAMESPACE(crypto_kem_keypair)
#define crypto_kem_enc     NAMESPACE(crypto_kem_enc)
#define crypto_kem_dec     NAMESPACE(crypto_kem_dec)

void hqc_kat_init(unsigned char *entropy_input, unsigned char *personalization_string, int security_strength);
void hqc_kat_release(void);

static void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
    size_t i;
    fprintf(fp, "%s", S);
    for (i = 0; i < L; i++) {
        fprintf(fp, "%02X", A[i]);
    }
    if (L == 0) {
        fprintf(fp, "00");
    }
    fprintf(fp, "\n");
}

int main(void) {
    uint8_t entropy_input[48];
    uint8_t seed[48];
    FILE *fh = stdout;
    uint8_t public_key[CRYPTO_PUBLICKEYBYTES];
    uint8_t secret_key[CRYPTO_SECRETKEYBYTES];
    uint8_t ciphertext[CRYPTO_CIPHERTEXTBYTES];
    uint8_t shared_secret_e[CRYPTO_BYTES];
    uint8_t shared_secret_d[CRYPTO_BYTES];
    int rc;

    for (uint8_t i = 0; i < 48; i++) {
        entropy_input[i] = i;
    }

    hqc_kat_init(entropy_input, NULL, 256);

    fprintf(fh, "count = 0\n");
    randombytes(seed, 48);
    fprintBstr(fh, "seed = ", seed, 48);

    hqc_kat_release();

    hqc_kat_init(seed, NULL, 256);

    rc = crypto_kem_keypair(public_key, secret_key);
    if (rc != 0) {
        fprintf(stderr, "[kat_kem] %s ERROR: crypto_kem_keypair failed!\n", CRYPTO_ALGNAME);
        return -1;
    }
    fprintBstr(fh, "pk = ", public_key, CRYPTO_PUBLICKEYBYTES);
    fprintBstr(fh, "sk = ", secret_key, CRYPTO_SECRETKEYBYTES);

    rc = crypto_kem_enc(ciphertext, shared_secret_e, public_key);
    if (rc != 0) {
        fprintf(stderr, "[kat_kem] %s ERROR: crypto_kem_enc failed!\n", CRYPTO_ALGNAME);
        return -2;
    }
    fprintBstr(fh, "ct = ", ciphertext, CRYPTO_CIPHERTEXTBYTES);
    fprintBstr(fh, "ss = ", shared_secret_e, CRYPTO_BYTES);

    rc = crypto_kem_dec(shared_secret_d, ciphertext, secret_key);
    if (rc != 0) {
        fprintf(stderr, "[kat_kem] %s ERROR: crypto_kem_dec failed!\n", CRYPTO_ALGNAME);
        return -3;
    }

    rc = memcmp(shared_secret_e, shared_secret_d, CRYPTO_BYTES);
    if (rc != 0) {
        fprintf(stderr, "[kat_kem] %s ERROR: shared secrets are not equal\n", CRYPTO_ALGNAME);
        return -4;
    }

    hqc_kat_release();

    return 0;

}

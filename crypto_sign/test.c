#include "api.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 15
#define MLEN 32

/* allocate a bit more for all keys and messages and
 * make sure it is not touched by the implementations.
 */
static void write_canary(unsigned char *d) {
  *((uint64_t *)d) = 0x0123456789ABCDEF;
}

static int check_canary(unsigned char *d) {
  if (*(uint64_t *)d != 0x0123456789ABCDEF)
    return -1;
  else
    return 0;
}
static int test_sign(void) {
  unsigned char pk[CRYPTO_PUBLICKEYBYTES + 16];
  unsigned char sk[CRYPTO_SECRETKEYBYTES + 16];
  unsigned char sm[MLEN + CRYPTO_BYTES + 16];
  unsigned char m[MLEN + 16];

  unsigned long long mlen;
  unsigned long long smlen;

  int i;
  write_canary(pk);
  write_canary(pk + sizeof(pk) - 8);
  write_canary(sk);
  write_canary(sk + sizeof(sk) - 8);
  write_canary(sm);
  write_canary(sm + sizeof(sm) - 8);
  write_canary(m);
  write_canary(m + sizeof(m) - 8);

  for (i = 0; i < NTESTS; i++) {
    crypto_sign_keypair(pk + 8, sk + 8);

    randombytes(m + 8, MLEN);
    crypto_sign(sm + 8, &smlen, m + 8, MLEN, sk + 8);

    // By relying on m == sm we prevent having to allocate CRYPTO_BYTES twice
    if (crypto_sign_open(sm + 8, &mlen, sm + 8, smlen, pk + 8)) {
      printf("ERROR Signature did not verify correctly!\n");
    } else if (check_canary(pk) || check_canary(pk + sizeof(pk) - 8) ||
               check_canary(sk) || check_canary(sk + sizeof(sk) - 8) ||
               check_canary(sm) || check_canary(sm + sizeof(sm) - 8) ||
               check_canary(m) || check_canary(m + sizeof(m) - 8)) {
      printf("ERROR canary overwritten\n");
    }
  }

  return 0;
}

static int test_wrong_pk(void) {
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char pk2[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk[CRYPTO_SECRETKEYBYTES];
  unsigned char sm[MLEN + CRYPTO_BYTES];
  unsigned char m[MLEN];

  unsigned long long mlen;
  unsigned long long smlen;

  int i;

  for (i = 0; i < NTESTS; i++) {
    crypto_sign_keypair(pk2, sk);

    crypto_sign_keypair(pk, sk);

    randombytes(m, MLEN);
    crypto_sign(sm, &smlen, m, MLEN, sk);

    // By relying on m == sm we prevent having to allocate CRYPTO_BYTES twice
    if (!crypto_sign_open(sm, &mlen, sm, smlen, pk2)) {
      printf("ERROR Signature did verify correctly under wrong public key!\n");
    }
  }

  return 0;
}

int main(void) {
  test_sign();
  test_wrong_pk();

  return 0;
}

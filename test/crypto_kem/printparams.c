#include <stdio.h>
#include "api.h"

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)

int main() {
    printf("{\n");
    printf("\t\"CRYPTO_SECRETKEYBYTES\": %u,\n",  NAMESPACE(CRYPTO_SECRETKEYBYTES));
    printf("\t\"CRYPTO_PUBLICKEYBYTES\": %u,\n",  NAMESPACE(CRYPTO_PUBLICKEYBYTES));
    printf("\t\"CRYPTO_CIPHERTEXTBYTES\": %u,\n", NAMESPACE(CRYPTO_CIPHERTEXTBYTES));
    printf("\t\"CRYPTO_BYTES\": %u,\n",           NAMESPACE(CRYPTO_BYTES));
    printf("\t\"CRYPTO_ALGNAME\": \"%s\"\n}\n",   NAMESPACE(CRYPTO_ALGNAME));
}

// bob.c (Client)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"
#include "../crypto_kem/ml-kem-768/clean/api.h"

// Bob 以為 Alice 在 8080 (其實這是 Eve)
#define TARGET_PORT 8080 

int main() {
    uint8_t pk[PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES];
    uint8_t ct[PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES];
    uint8_t ss[PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES];

    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(TARGET_PORT);
    
    // 假設都在本機
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        perror("Invalid address");
        return -1;
    }

    printf("\n[Bob(Client)] Connecting to port %d...\n", TARGET_PORT);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed (Is Eve running?)");
        return -1;
    }

    // 1. 接收 Public Key (Bob 以為是 Alice 的，其實是 Eve 的)
    printf("[Bob] Receiving Public Key...\n");
    recv_all(sock, pk, sizeof(pk));

    // 2. Encaps (封裝)
    printf("[Bob] Encapsulating (Generating Secret)...\n");
    PQCLEAN_MLKEM768_CLEAN_crypto_kem_enc(ct, ss, pk);

    // 3. 發送 Ciphertext
    printf("[Bob] Sending Ciphertext...\n");
    send_all(sock, ct, sizeof(ct));

    printf(">>> [Bob] Shared Secret Generated:   ");
    for(int i=0; i<8; i++) printf("%02X ", ss[i]);
    printf("...\n");

    close(sock);
    return 0;
}
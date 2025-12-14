// alice.c (Server)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"
#include "../crypto_kem/ml-kem-768/clean/api.h"


#define PORT 8081

int main() {
    uint8_t pk[PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[PQCLEAN_MLKEM768_CLEAN_CRYPTO_SECRETKEYBYTES];
    uint8_t ct[PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES];
    uint8_t ss[PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES];

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    // 1. 建立 Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("\n[Alice(Server)] Listening on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 2. KeyGen
    printf("[Alice] Generating Key Pair...\n");
    PQCLEAN_MLKEM768_CLEAN_crypto_kem_keypair(pk, sk);

    // 3. 發送 Public Key
    printf("[Alice] Sending Public Key...\n");
    send_all(new_socket, pk, sizeof(pk));

    // 4. 接收 Ciphertext
    printf("[Alice] Waiting for Ciphertext...\n");
    recv_all(new_socket, ct, sizeof(ct));

    // 5. Decaps (解出共享密鑰)
    printf("[Alice] Decapsulating...\n");
    PQCLEAN_MLKEM768_CLEAN_crypto_kem_dec(ss, ct, sk);

    printf(">>> [Alice] Shared Secret Established: ");
    for(int i=0; i<8; i++) printf("%02X ", ss[i]);
    printf("...\n");

    close(new_socket);
    close(server_fd);
    return 0;
}
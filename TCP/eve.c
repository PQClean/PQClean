// eve.c (Man-in-the-Middle)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"
#include "../crypto_kem/ml-kem-768/clean/api.h"

#define LISTEN_PORT 8080 // 騙 Bob 這裡就是 Alice
#define REAL_SERVER_PORT 8081 // 真正的 Alice 在這

int main() {
    // 變數準備
    // Key Pair: Eve 偽造給 Bob 的
    uint8_t pk_eve[PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES];
    uint8_t sk_eve[PQCLEAN_MLKEM768_CLEAN_CRYPTO_SECRETKEYBYTES];
    
    // Key: 真正的 Alice 的公鑰
    uint8_t pk_alice[PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES];

    // Ciphertexts
    uint8_t ct_from_bob[PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES];
    uint8_t ct_for_alice[PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES];

    // Secrets (Eve 會拿到兩份不同的!)
    uint8_t ss_with_bob[PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES];
    uint8_t ss_with_alice[PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES];

    // ==========================================
    // STEP 1: 連線到真正的 Alice (先準備好後路)
    // ==========================================
    int sock_alice = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in alice_addr;
    alice_addr.sin_family = AF_INET;
    alice_addr.sin_port = htons(REAL_SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &alice_addr.sin_addr);

    printf("\n[Eve] Connecting to REAL Alice (%d)...\n", REAL_SERVER_PORT);
    if (connect(sock_alice, (struct sockaddr *)&alice_addr, sizeof(alice_addr)) < 0) {
        perror("[Eve] Connect to Alice failed");
        exit(1);
    }
    
    // 接收 Alice 的公鑰 (Eve 把這個收起來，不給 Bob)
    recv_all(sock_alice, pk_alice, sizeof(pk_alice));
    printf("[Eve] Got Alice's PK. Storing it.\n");

    // ==========================================
    // STEP 2: 啟動假 Server 等待 Bob
    // ==========================================
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in eve_addr;
    eve_addr.sin_family = AF_INET;
    eve_addr.sin_addr.s_addr = INADDR_ANY;
    eve_addr.sin_port = htons(LISTEN_PORT);
    
    bind(listen_fd, (struct sockaddr *)&eve_addr, sizeof(eve_addr));
    listen(listen_fd, 3);
    
    printf("[Eve] Listening on port %d (Waiting for Bob)...\n", LISTEN_PORT);
    
    int sock_bob = accept(listen_fd, NULL, NULL);
    printf("[Eve] Bob connected!\n");

    // ==========================================
    // STEP 3: 攻擊開始 (Swap Public Keys)
    // ==========================================
    
    // Eve 生成自己的假 Key
    printf("[Eve] Generating FAKE Key Pair...\n");
    PQCLEAN_MLKEM768_CLEAN_crypto_kem_keypair(pk_eve, sk_eve);

    // 把 Eve 的 PK 發給 Bob (Bob 以為這是 Alice 的)
    printf("[Eve] Sending FAKE Public Key to Bob...\n");
    send_all(sock_bob, pk_eve, sizeof(pk_eve));

    // 接收 Bob 的密文
    printf("[Eve] Receiving Ciphertext from Bob...\n");
    recv_all(sock_bob, ct_from_bob, sizeof(ct_from_bob));

    // ==========================================
    // STEP 4: 解密並竊取 (Decaps & Re-Encaps)
    // ==========================================

    // 1. 解開 Bob 的密文 -> 拿到 Eve-Bob 的密鑰
    PQCLEAN_MLKEM768_CLEAN_crypto_kem_dec(ss_with_bob, ct_from_bob, sk_eve);
    printf(">>> [Eve] STOLEN Secret (with Bob):   ");
    for(int i=0; i<8; i++) printf("%02X ", ss_with_bob[i]);
    printf("...\n");

    // 2. 用真正的 Alice 公鑰封裝 -> 產生 Eve-Alice 的密鑰
    PQCLEAN_MLKEM768_CLEAN_crypto_kem_enc(ct_for_alice, ss_with_alice, pk_alice);
    
    // 3. 把新密文發給 Alice (騙 Alice 這是 Bob 發的)
    send_all(sock_alice, ct_for_alice, sizeof(ct_for_alice));
    
    printf(">>> [Eve] ESTABLISHED Secret (with Alice): ");
    for(int i=0; i<8; i++) printf("%02X ", ss_with_alice[i]);
    printf("...\n");

    printf("\n[Eve] Attack Complete.\n");
    close(sock_bob);
    close(sock_alice);
    close(listen_fd);
    return 0;
}
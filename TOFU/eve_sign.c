#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "api.h"
#include "../crypto_sign/ml-dsa-44/clean/api.h"

#define PORT 8080
#define EVE_DSA_FILE "eve_dsa.key" // Eve 的假冒身分

// === 定義同 Server ===
#define KEM_KEYPAIR PQCLEAN_MLKEM768_CLEAN_crypto_kem_keypair
#define KEM_PK_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES
#define KEM_SK_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_SECRETKEYBYTES
#define KEM_CT_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES
#define KEM_SS_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES

#define DSA_KEYPAIR PQCLEAN_MLDSA44_CLEAN_crypto_sign_keypair
#define DSA_SIGN    PQCLEAN_MLDSA44_CLEAN_crypto_sign
#define DSA_PK_BYTES PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES
#define DSA_SK_BYTES PQCLEAN_MLDSA44_CLEAN_CRYPTO_SECRETKEYBYTES
#define DSA_BYTES    PQCLEAN_MLDSA44_CLEAN_CRYPTO_BYTES

// Eve 生成她自己的（假）身分
void get_fake_identity(uint8_t *pk, uint8_t *sk) {
    FILE *fp = fopen(EVE_DSA_FILE, "rb");
    if (fp) {
        printf("[Eve] Loading existing FAKE identity keys...\n");
        fread(pk, 1, DSA_PK_BYTES, fp);
        fread(sk, 1, DSA_SK_BYTES, fp);
        fclose(fp);
    } else {
        printf("[Eve] Generating NEW FAKE identity keys...\n");
        DSA_KEYPAIR(pk, sk); // 這是合法的 DSA Key，但不是 Server 的 Key
        fp = fopen(EVE_DSA_FILE, "wb");
        fwrite(pk, 1, DSA_PK_BYTES, fp);
        fwrite(sk, 1, DSA_SK_BYTES, fp);
        fclose(fp);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    uint8_t dsa_pk[DSA_PK_BYTES];
    uint8_t dsa_sk[DSA_SK_BYTES];
    uint8_t kem_pk[KEM_PK_BYTES];
    uint8_t kem_sk[KEM_SK_BYTES];
    uint8_t kem_ct[KEM_CT_BYTES]; // Eve 收到的密文，雖然她解不開之前的，但這次連線是她建立的
    uint8_t kem_ss[KEM_SS_BYTES];

    size_t sm_len;
    uint8_t signed_msg[DSA_BYTES + KEM_PK_BYTES]; 

    // 1. 準備假身分
    get_fake_identity(dsa_pk, dsa_sk);

    // 2. 搶佔 Port 8080
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("[Eve] Socket failed");
        exit(1);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("[Eve] Bind failed (Is the real server still running?)");
        exit(1);
    }
    if (listen(server_fd, 3) < 0) {
        perror("[Eve] Listen failed");
        exit(1);
    }

    printf("[Eve] Rogue Server started on port %d. Waiting for victims...\n", PORT);

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("[Eve] Accept failed");
            continue;
        }

        printf("[Eve] Victim connected! Initiating fake handshake...\n");
        
        // 3. 正常生成 KEM Key (數學上沒問題)
        KEM_KEYPAIR(kem_pk, kem_sk);
        
        // 4. 用 Eve 的私鑰簽章 (問題就在這：Client 不信任這把鑰匙)
        DSA_SIGN(signed_msg, &sm_len, kem_pk, KEM_PK_BYTES, dsa_sk);

        // 5. 發送假身分與簽章
        send(new_socket, dsa_pk, DSA_PK_BYTES, 0); // Client 這裡檢查就會爆炸
        send(new_socket, &sm_len, sizeof(size_t), 0);
        send(new_socket, signed_msg, sm_len, 0);

        printf("[Eve] Fake identity and keys sent. Waiting for response...\n");

        // 如果 Client 沒有檢查身分，Eve 就能繼續下去
        int valread = read(new_socket, kem_ct, KEM_CT_BYTES);
        if (valread == KEM_CT_BYTES) {
            printf("[Eve] HAHA! Victim accepted our key! Decapsulating...\n");
            // 這裡 Eve 甚至可以成功建立連線，因為這是一個全新的會話
            // 證明了如果沒有 Authentication，MitM 是完全可行的
        } else {
            printf("[Eve] Connection closed by remote host (Victim likely detected us).\n");
        }

        close(new_socket);
    }
}
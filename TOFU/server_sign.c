#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../crypto_kem/ml-kem-768/clean/api.h"
#include "../crypto_sign/ml-dsa-44/clean/api.h" 

#define PORT 8080
#define DSA_KEY_FILE "server_dsa.key"

// === KEM 定義 (ML-KEM-768) ===
#define KEM_KEYPAIR PQCLEAN_MLKEM768_CLEAN_crypto_kem_keypair
#define KEM_DEC     PQCLEAN_MLKEM768_CLEAN_crypto_kem_dec
#define KEM_PK_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES
#define KEM_SK_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_SECRETKEYBYTES
#define KEM_CT_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES
#define KEM_SS_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES

// === DSA 定義 (ML-DSA-44) ===
#define DSA_KEYPAIR PQCLEAN_MLDSA44_CLEAN_crypto_sign_keypair
#define DSA_SIGN    PQCLEAN_MLDSA44_CLEAN_crypto_sign
#define DSA_PK_BYTES PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES
#define DSA_SK_BYTES PQCLEAN_MLDSA44_CLEAN_CRYPTO_SECRETKEYBYTES
#define DSA_BYTES    PQCLEAN_MLDSA44_CLEAN_CRYPTO_BYTES // 簽章最大長度

void get_dsa_keypair(uint8_t *pk, uint8_t *sk) {
    FILE *fp = fopen(DSA_KEY_FILE, "rb");
    if (fp) {
        printf("[Server] Loading DSA identity keys...\n");
        fread(pk, 1, DSA_PK_BYTES, fp);
        fread(sk, 1, DSA_SK_BYTES, fp);
        fclose(fp);
    } else {
        printf("[Server] Generating NEW DSA identity keys...\n");
        DSA_KEYPAIR(pk, sk);
        fp = fopen(DSA_KEY_FILE, "wb");
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

    // DSA 變數 (長期身分)
    uint8_t dsa_pk[DSA_PK_BYTES];
    uint8_t dsa_sk[DSA_SK_BYTES];

    // KEM 變數 (臨時連線)
    uint8_t kem_pk[KEM_PK_BYTES];
    uint8_t kem_sk[KEM_SK_BYTES];
    uint8_t kem_ct[KEM_CT_BYTES];
    uint8_t kem_ss[KEM_SS_BYTES];

    // 簽章變數
    // signed_msg = Signature + Message (KEM PK)
    size_t sm_len;
    uint8_t signed_msg[DSA_BYTES + KEM_PK_BYTES]; 

    // 1. 準備長期 DSA Key
    get_dsa_keypair(dsa_pk, dsa_sk);

    // 2. 建立 Socket (略，同前)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) exit(1);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("[Server] Listening (Authenticated Mode)...\n");

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        // 3. 生成「臨時」KEM Key (每次連線都不同)
        KEM_KEYPAIR(kem_pk, kem_sk);
        
        // 4. 用 DSA 私鑰簽署 KEM 公鑰
        // param: signed_message_buffer, output_len_ptr, message, message_len, signing_key
        DSA_SIGN(signed_msg, &sm_len, kem_pk, KEM_PK_BYTES, dsa_sk);

        printf("[Server] Generated Ephemeral KEM Key & Signed it.\n");

        // 5. 傳送協議封包
        // 封包結構：[DSA_PK (身分)] + [Signed_Message_Len (4 bytes)] + [Signed_Message (簽章+KEM公鑰)]
        
        // (A) 傳送身分 (DSA PK) 讓 Client 進行 TOFU 檢查
        send(new_socket, dsa_pk, DSA_PK_BYTES, 0);

        // (B) 傳送簽章訊息長度 (因為簽章長度可能會變，雖然 Dilithium 是固定的，但好習慣是傳長度)
        send(new_socket, &sm_len, sizeof(size_t), 0);

        // (C) 傳送簽章後的 KEM 公鑰
        send(new_socket, signed_msg, sm_len, 0);

        // 6. 等待 Client 回傳 KEM 密文
        if (read(new_socket, kem_ct, KEM_CT_BYTES) == KEM_CT_BYTES) {
            KEM_DEC(kem_ss, kem_ct, kem_sk);
            printf("[Server] Shared Secret Established: %02x...\n", kem_ss[0]);
        }

        close(new_socket);
    }
}
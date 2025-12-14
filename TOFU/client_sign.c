#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../crypto_kem/ml-kem-768/clean/api.h"
#include "../crypto_sign/ml-dsa-44/clean/api.h"

#define PORT 8080
#define KNOWN_HOST_FILE "known_hosts_dsa.dat"

// Macro 定義同 Server
#define KEM_ENC     PQCLEAN_MLKEM768_CLEAN_crypto_kem_enc
#define KEM_PK_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_PUBLICKEYBYTES
#define KEM_CT_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_CIPHERTEXTBYTES
#define KEM_SS_BYTES PQCLEAN_MLKEM768_CLEAN_CRYPTO_BYTES

#define DSA_OPEN    PQCLEAN_MLDSA44_CLEAN_crypto_sign_open
#define DSA_PK_BYTES PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES
#define DSA_BYTES    PQCLEAN_MLDSA44_CLEAN_CRYPTO_BYTES

// 檢查 DSA 身分 (TOFU)
void check_identity(uint8_t *received_pk) {
    FILE *fp = fopen(KNOWN_HOST_FILE, "rb");
    uint8_t saved_pk[DSA_PK_BYTES];

    if (fp) {
        fread(saved_pk, 1, DSA_PK_BYTES, fp);
        fclose(fp);
        if (memcmp(received_pk, saved_pk, DSA_PK_BYTES) != 0) {
            printf("[ALARM] Server Identity CHANGED! Possible MitM!\n");
            exit(1);
        }
        printf("[Client] Server Identity Verified (TOFU).\n");
    } else {
        printf("[Client] First time seeing this Server Identity (DSA). Trusting.\n");
        fp = fopen(KNOWN_HOST_FILE, "wb");
        fwrite(received_pk, 1, DSA_PK_BYTES, fp);
        fclose(fp);
    }
}

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *ip = "127.0.0.1";
    if(argc > 1) ip = (char*)argv[1];

    uint8_t dsa_pk[DSA_PK_BYTES];
    uint8_t kem_pk[KEM_PK_BYTES];
    uint8_t kem_ct[KEM_CT_BYTES];
    uint8_t kem_ss[KEM_SS_BYTES];

    // 接收緩衝區
    size_t sm_len;
    uint8_t signed_msg[DSA_BYTES + KEM_PK_BYTES];
    size_t msg_len; // 解開簽章後的訊息長度

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) return -1;

    // 1. 接收 Server 的身分 (DSA PK)
    read(sock, dsa_pk, DSA_PK_BYTES);
    
    // 2. 進行 TOFU 檢查
    check_identity(dsa_pk);

    // 3. 接收簽章後的封包長度 與 內容
    read(sock, &sm_len, sizeof(size_t));
    int total_read = 0;
    while(total_read < (int)sm_len) {
        total_read += read(sock, signed_msg + total_read, sm_len - total_read);
    }

    // 4. 驗證簽章 (Verify)
    // crypto_sign_open 會驗證簽章，如果成功，會把原始訊息 (KEM PK) 放入 kem_pk
    // 注意：PQClean 的 open 函數通常參數是 (output_msg, output_len, signed_msg, signed_len, public_key)
    // 這裡我們直接把 output 寫到 kem_pk 變數中
    // 為了安全，最好準備一個夠大的 buffer，但這裡我們知道 message 就是 KEM_PK
    uint8_t buffer[DSA_BYTES + KEM_PK_BYTES]; 
    
    if (DSA_OPEN(buffer, &msg_len, signed_msg, sm_len, dsa_pk) != 0) {
        printf("[ERROR] Signature Verification FAILED! The KEM key is forged.\n");
        exit(1);
    }

    // 驗證成功，從 buffer 提取 KEM PK
    // 根據 API，buffer 裡現在放的是純 message
    memcpy(kem_pk, buffer, KEM_PK_BYTES);

    printf("[Client] Signature Verified! Ephemeral KEM Key is authentic.\n");

    // 5. 進行 ML-KEM 封裝
    KEM_ENC(kem_ct, kem_ss, kem_pk);

    // 6. 回傳密文
    send(sock, kem_ct, KEM_CT_BYTES, 0);
    printf("[Client] Encapsulation done. Shared Secret: %02x...\n", kem_ss[0]);

    close(sock);
    return 0;
}
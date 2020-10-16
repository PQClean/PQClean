#include "./polymul/toom-cook_4way.c"
#include "SABER_indcpa.h"
#include "SABER_params.h"
#include "api.h"
#include "cbd.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "randombytes.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//#include "randombytes.h"
//#include "./polymul/toom_cook_4/toom-cook_4way.c"

#define h1 4 //2^(EQ-EP-1)

#define h2 ( (1<<(SABER_EP-2)) - (1<<(SABER_EP-SABER_ET-1)) + (1<<(SABER_EQ-SABER_EP-1)) )


static void POL2MSG(uint8_t *message_dec, const uint16_t *message_dec_unpacked) {
    int32_t i, j;

    for (j = 0; j < SABER_KEYBYTES; j++) {
        message_dec[j] = 0;
        for (i = 0; i < 8; i++) {
            message_dec[j] = message_dec[j] | (message_dec_unpacked[j * 8 + i] << i);
        }
    }
}

/*-----------------------------------------------------------------------------------
    This routine generates a=[Matrix K x K] of 256-coefficient polynomials
-------------------------------------------------------------------------------------*/

static void GenMatrix(polyvec *a, const uint8_t *seed) {
    uint8_t buf[SABER_K * SABER_K * 13 * SABER_N / 8];

    uint16_t temp_ar[SABER_N];

    int i, j, k;
    uint16_t mod = (SABER_Q - 1);

    shake128(buf, sizeof(buf), seed, SABER_SEEDBYTES);

    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_K; j++) {
            PQCLEAN_FIRESABER_AVX2_BS2POLq(temp_ar, buf + (i * SABER_K + j) * 13 * SABER_N / 8);
            for (k = 0; k < SABER_N; k++) {
                a[i].vec[j].coeffs[k] = (temp_ar[k])& mod ;
            }
        }
    }
}

static void GenSecret(uint16_t r[SABER_K][SABER_N], const uint8_t *seed) {

    uint32_t i;

    uint8_t buf[SABER_MU * SABER_N * SABER_K / 8];

    shake128(buf, sizeof(buf), seed, SABER_NOISESEEDBYTES);

    for (i = 0; i < SABER_K; i++) {
        PQCLEAN_FIRESABER_AVX2_cbd(r[i], buf + i * SABER_MU * SABER_N / 8);
    }
}

//********************************matrix-vector mul routines*****************************************************
static void matrix_vector_mul(__m256i a1_avx_combined[NUM_POLY][NUM_POLY][AVX_N1], __m256i b_bucket[NUM_POLY][SCHB_N * 4], __m256i res_avx[NUM_POLY][AVX_N1], int isTranspose) {
    int64_t i, j;

    __m256i c_bucket[2 * SCM_SIZE * 4]; //Holds results for 9 Karatsuba at a time

    for (i = 0; i < NUM_POLY; i++) {
        for (j = 0; j < NUM_POLY; j++) {

            if (isTranspose == 0) {
                toom_cook_4way_avx_n1(a1_avx_combined[i][j], b_bucket[j], c_bucket, j);
            } else {
                toom_cook_4way_avx_n1(a1_avx_combined[j][i], b_bucket[j], c_bucket, j);
            }
        }

        TC_interpol(c_bucket, res_avx[i]);
    }

}

static void vector_vector_mul(__m256i a_avx[NUM_POLY][AVX_N1], __m256i b_bucket[NUM_POLY][SCHB_N * 4], __m256i res_avx[AVX_N1]) {

    int64_t i;

    __m256i c_bucket[2 * SCM_SIZE * 4]; //Holds results for 9 Karatsuba at a time

    for (i = 0; i < NUM_POLY; i++) {
        toom_cook_4way_avx_n1(a_avx[i], b_bucket[i], c_bucket, i);
    }
    TC_interpol(c_bucket, res_avx);
}

//********************************matrix-vector mul routines*****************************************************

void PQCLEAN_FIRESABER_AVX2_indcpa_kem_keypair(uint8_t *pk, uint8_t *sk) {

    polyvec a[SABER_K];

    uint16_t skpv1[SABER_K][SABER_N];



    uint8_t seed[SABER_SEEDBYTES];
    uint8_t noiseseed[SABER_COINBYTES];
    int32_t i, j, k;


//--------------AVX declaration------------------

    __m256i sk_avx[SABER_K][SABER_N / 16];
    __m256i mod;
    __m256i res_avx[SABER_K][SABER_N / 16];
    __m256i a_avx[SABER_K][SABER_K][SABER_N / 16];
    //__m256i acc[2*SABER_N/16];

    mod = _mm256_set1_epi16(SABER_Q - 1);

    __m256i b_bucket[NUM_POLY][SCHB_N * 4];

//--------------AVX declaration ends------------------

    randombytes(seed, SABER_SEEDBYTES);

    shake128(seed, SABER_SEEDBYTES, seed, SABER_SEEDBYTES); // for not revealing system RNG state
    randombytes(noiseseed, SABER_COINBYTES);


    GenMatrix(a, seed); //sample matrix A

    GenSecret(skpv1, noiseseed);


// Load sk into avx vectors
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N / 16; j++) {
            sk_avx[i][j] = _mm256_loadu_si256 ((__m256i const *) (&skpv1[i][j * 16]));
        }

    }

    // Load a into avx vectors
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_K; j++) {
            for (k = 0; k < SABER_N / 16; k++) {
                a_avx[i][j][k] = _mm256_loadu_si256 ((__m256i const *) (&a[i].vec[j].coeffs[k * 16]));
            }
        }
    }



    //------------------------do the matrix vector multiplication and rounding------------

    for (j = 0; j < NUM_POLY; j++) {
        TC_eval(sk_avx[j], b_bucket[j]);
    }
    matrix_vector_mul(a_avx, b_bucket, res_avx, 1);// Matrix-vector multiplication; Matrix in transposed order

    // Now truncation


    for (i = 0; i < SABER_K; i++) { //shift right EQ-EP bits
        for (j = 0; j < SABER_N / 16; j++) {
            res_avx[i][j] = _mm256_add_epi16 (res_avx[i][j], _mm256_set1_epi16(h1));
            res_avx[i][j] = _mm256_srli_epi16 (res_avx[i][j], (SABER_EQ - SABER_EP) );
            res_avx[i][j] = _mm256_and_si256 (res_avx[i][j], mod);
        }
    }

    //------------------Pack sk into byte string-------

    PQCLEAN_FIRESABER_AVX2_POLVEC2BS(sk, (const uint16_t (*)[SABER_N])skpv1, SABER_Q);

    //------------------Pack pk into byte string-------

    for (i = 0; i < SABER_K; i++) { // reuses skpv1[] for unpacking avx of public-key
        for (j = 0; j < SABER_N / 16; j++) {
            _mm256_maskstore_epi32 ((int *) (skpv1[i] + j * 16), _mm256_set1_epi32(-1), res_avx[i][j]);
        }
    }
    PQCLEAN_FIRESABER_AVX2_POLVEC2BS(pk, (const uint16_t (*)[SABER_N])skpv1, SABER_P); // load the public-key into pk byte string


    for (i = 0; i < SABER_SEEDBYTES; i++) { // now load the seedbytes in PK. Easy since seed bytes are kept in byte format.
        pk[SABER_POLYVECCOMPRESSEDBYTES + i] = seed[i];
    }

}


void PQCLEAN_FIRESABER_AVX2_indcpa_kem_enc(uint8_t ciphertext[SABER_BYTES_CCA_DEC], const uint8_t m[SABER_KEYBYTES], const uint8_t noiseseed[SABER_NOISESEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES]) {


    uint32_t i, j, k;
    polyvec a[SABER_K];     // skpv;
    uint8_t seed[SABER_SEEDBYTES];
    uint16_t pkcl[SABER_K][SABER_N];    //public key of received by the client


    uint16_t skpv1[SABER_K][SABER_N];
    uint16_t temp[SABER_K][SABER_N];
    uint16_t message[SABER_KEYBYTES * 8];

    uint8_t msk_c[SABER_SCALEBYTES_KEM];

    //--------------AVX declaration------------------

    __m256i sk_avx[SABER_K][SABER_N / 16];
    __m256i mod, mod_p;
    __m256i res_avx[SABER_K][SABER_N / 16];
    __m256i vprime_avx[SABER_N / 16];
    __m256i a_avx[SABER_K][SABER_K][SABER_N / 16];
    //__m256i acc[2*SABER_N/16];

    __m256i pkcl_avx[SABER_K][SABER_N / 16];

    __m256i message_avx[SABER_N / 16];

    mod = _mm256_set1_epi16(SABER_Q - 1);
    mod_p = _mm256_set1_epi16(SABER_P - 1);



    __m256i b_bucket[NUM_POLY][SCHB_N * 4];

    //--------------AVX declaration ends------------------
    for (i = 0; i < SABER_SEEDBYTES; i++) { // Load the seedbytes in the client seed from PK.
        seed[i] = pk[ SABER_POLYVECCOMPRESSEDBYTES + i];
    }

    GenMatrix(a, seed);
    GenSecret(skpv1, noiseseed);

    // ----------- Load skpv1 into avx vectors ----------
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N / 16; j++) {
            sk_avx[i][j] = _mm256_loadu_si256 ((__m256i const *) (&skpv1[i][j * 16]));
        }
    }

    // ----------- Load skpv1 into avx vectors ----------
    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_K; j++) {
            for (k = 0; k < SABER_N / 16; k++) {
                a_avx[i][j][k] = _mm256_loadu_si256 ((__m256i const *) (&a[i].vec[j].coeffs[k * 16]));
            }
        }
    }
    //-----------------matrix-vector multiplication and rounding

    for (j = 0; j < NUM_POLY; j++) {
        TC_eval(sk_avx[j], b_bucket[j]);
    }
    matrix_vector_mul(a_avx, b_bucket, res_avx, 0);// Matrix-vector multiplication; Matrix in normal order

    // Now truncation

    for (i = 0; i < SABER_K; i++) { //shift right EQ-EP bits
        for (j = 0; j < SABER_N / 16; j++) {
            res_avx[i][j] = _mm256_add_epi16 (res_avx[i][j], _mm256_set1_epi16(h1));
            res_avx[i][j] = _mm256_srli_epi16 (res_avx[i][j], (SABER_EQ - SABER_EP) );
            res_avx[i][j] = _mm256_and_si256 (res_avx[i][j], mod);

        }
    }


    //-----this result should be put in b_prime for later use in server.
    for (i = 0; i < SABER_K; i++) { // first store in 16 bit arrays
        for (j = 0; j < SABER_N / 16; j++) {
            _mm256_maskstore_epi32 ((int *)(temp[i] + j * 16), _mm256_set1_epi32(-1), res_avx[i][j]);
        }
    }

    PQCLEAN_FIRESABER_AVX2_POLVEC2BS(ciphertext, (const uint16_t (*)[SABER_N])temp, SABER_P); // Pack b_prime into ciphertext byte string

//**************client matrix-vector multiplication ends******************//

    //------now calculate the v'

    //-------unpack the public_key
    PQCLEAN_FIRESABER_AVX2_BS2POLVEC(pkcl, pk, SABER_P);

    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N / 16; j++) {
            pkcl_avx[i][j] = _mm256_loadu_si256 ((__m256i const *) (&pkcl[i][j * 16]));
        }
    }

    // InnerProduct
    //for(k=0;k<SABER_N/16;k++){
    //  vprime_avx[k]=_mm256_xor_si256(vprime_avx[k],vprime_avx[k]);
    //}

    // vector-vector scalar multiplication with mod p

    vector_vector_mul(pkcl_avx, b_bucket, vprime_avx);

    // Computation of v'+h1
    for (i = 0; i < SABER_N / 16; i++) { //adding h1
        vprime_avx[i] = _mm256_add_epi16(vprime_avx[i], _mm256_set1_epi16(h1));
    }

    // unpack m;
    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            message[8 * j + i] = ((m[j] >> i) & 0x01);
        }
    }
    // message encoding
    for (i = 0; i < SABER_N / 16; i++) {
        message_avx[i] = _mm256_loadu_si256 ((__m256i const *) (&message[i * 16]));
        message_avx[i] = _mm256_slli_epi16 (message_avx[i], (SABER_EP - 1) );
    }

    // SHIFTRIGHT(v'+h1-m mod p, EP-ET)
    for (k = 0; k < SABER_N / 16; k++) {
        vprime_avx[k] = _mm256_sub_epi16(vprime_avx[k], message_avx[k]);
        vprime_avx[k] = _mm256_and_si256(vprime_avx[k], mod_p);
        vprime_avx[k] = _mm256_srli_epi16 (vprime_avx[k], (SABER_EP - SABER_ET) );
    }

    // Unpack avx
    for (j = 0; j < SABER_N / 16; j++) {
        _mm256_maskstore_epi32 ((int *) (temp[0] + j * 16), _mm256_set1_epi32(-1), vprime_avx[j]);
    }

    PQCLEAN_FIRESABER_AVX2_SABER_pack_6bit(msk_c, temp[0]);


    for (j = 0; j < SABER_SCALEBYTES_KEM; j++) {
        ciphertext[SABER_CIPHERTEXTBYTES + j] = msk_c[j];
    }

}


void PQCLEAN_FIRESABER_AVX2_indcpa_kem_dec(uint8_t m[SABER_KEYBYTES], const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC]) {

    uint32_t i, j;
    uint16_t sksv[SABER_K][SABER_N]; //secret key of the server
    uint16_t pksv[SABER_K][SABER_N];
    uint16_t message_dec_unpacked[SABER_KEYBYTES * 8];  // one element containes on decrypted bit;
    uint8_t scale_ar[SABER_SCALEBYTES_KEM];
    uint16_t op[SABER_N];

    //--------------AVX declaration------------------


    //__m256i mod_p;

    __m256i v_avx[SABER_N / 16];

    //__m256i acc[2*SABER_N/16];

    __m256i sksv_avx[SABER_K][SABER_N / 16];
    __m256i pksv_avx[SABER_K][SABER_N / 16];

    //mod_p=_mm256_set1_epi16(SABER_P-1);

    __m256i b_bucket[NUM_POLY][SCHB_N * 4];
    //--------------AVX declaration ends------------------

    //-------unpack the public_key

    PQCLEAN_FIRESABER_AVX2_BS2POLVEC(sksv, sk, SABER_Q); //sksv is the secret-key
    PQCLEAN_FIRESABER_AVX2_BS2POLVEC(pksv, ciphertext, SABER_P); //pksv is the ciphertext

    for (i = 0; i < SABER_K; i++) {
        for (j = 0; j < SABER_N / 16; j++) {
            sksv_avx[i][j] = _mm256_loadu_si256 ((__m256i const *) (&sksv[i][j * 16]));
            pksv_avx[i][j] = _mm256_loadu_si256 ((__m256i const *) (&pksv[i][j * 16]));
        }
    }

    for (i = 0; i < SABER_N / 16; i++) {
        v_avx[i] = _mm256_xor_si256(v_avx[i], v_avx[i]);
    }


    // InnerProduct(b', s, mod p)

    for (j = 0; j < NUM_POLY; j++) {
        TC_eval(sksv_avx[j], b_bucket[j]);
    }

    vector_vector_mul(pksv_avx, b_bucket, v_avx);

    for (i = 0; i < SABER_N / 16; i++) {
        _mm256_maskstore_epi32 ((int *)(message_dec_unpacked + i * 16), _mm256_set1_epi32(-1), v_avx[i]);
    }


    for (i = 0; i < SABER_SCALEBYTES_KEM; i++) {
        scale_ar[i] = ciphertext[SABER_CIPHERTEXTBYTES + i];
    }

    PQCLEAN_FIRESABER_AVX2_SABER_un_pack6bit(op, scale_ar);


    //addition of h2
    for (i = 0; i < SABER_N; i++) {
        message_dec_unpacked[i] = ( ( message_dec_unpacked[i] + h2 - (op[i] << (SABER_EP - SABER_ET)) ) & (SABER_P - 1) ) >> (SABER_EP - 1);
    }


    POL2MSG(m, message_dec_unpacked);
}

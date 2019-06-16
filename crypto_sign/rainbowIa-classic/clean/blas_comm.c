/// @file blas_comm.c
/// @brief The standard implementations for blas_comm.h
///

#include "blas.h"
#include "blas_comm.h"
#include "gf.h"
#include "rainbow_config.h"

#include <stdint.h>
#include <string.h>


void PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(uint8_t *b, unsigned _num_byte) {
    gf256v_add(b, b, _num_byte);
}
#ifdef _USE_GF16

/// @brief get an element from GF(16) vector .
///
/// @param[in]  a         - the input vector a.
/// @param[in]  i         - the index in the vector a.
/// @return  the value of the element.
///
uint8_t PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(const uint8_t *a, unsigned i) {
    uint8_t r = a[i >> 1];
    uint8_t r0 = r & 0xf;
    uint8_t r1 = r >> 4;
    uint8_t m = (uint8_t)(-((int8_t)i & 1));
    return (uint8_t)((r1 & m) | ((~m)&r0));
}

/// @brief set an element for a GF(16) vector .
///
/// @param[in,out]   a   - the vector a.
/// @param[in]  i        - the index in the vector a.
/// @param[in]  v        - the value for the i-th element in vector a.
/// @return  the value of the element.
///
static uint8_t PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_set_ele(uint8_t *a, unsigned i, uint8_t v) {
    uint8_t m = (uint8_t) (0xf ^ (-((int8_t)i & 1))); ///  1--> 0xf0 , 0--> 0x0f
    uint8_t ai_remaining = (uint8_t) (a[i >> 1] & (~m)); /// erase
    a[i >> 1] = (uint8_t) (ai_remaining | (m & (v << 4)) | (m & v & 0xf)); /// set
    return v;
}

static void gf16mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(c, n_A_vec_byte);
    for (unsigned i = 0; i < n_A_width; i++) {
        uint8_t bb = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(b, i);
        gf16v_madd(c, matA, bb, n_A_vec_byte);
        matA += n_A_vec_byte;
    }
}

void PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_mul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned len_vec) {
    unsigned n_vec_byte = (len_vec + 1) / 2;
    for (unsigned k = 0; k < len_vec; k++) {
        PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(c, n_vec_byte);
        const uint8_t *bk = b + n_vec_byte * k;
        for (unsigned i = 0; i < len_vec; i++) {
            uint8_t bb = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(bk, i);
            gf16v_madd(c, a + n_vec_byte * i, bb, n_vec_byte);
        }
        c += n_vec_byte;
    }
}

static
unsigned gf16mat_gauss_elim_ref(uint8_t *mat, unsigned h, unsigned w) {
    unsigned n_w_byte = (w + 1) / 2;
    unsigned r8 = 1;
    for (unsigned i = 0; i < h; i++) {
        unsigned offset_byte = i >> 1;
        uint8_t *ai = mat + n_w_byte * i;
        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *aj = mat + n_w_byte * j;
            gf256v_predicated_add(ai + offset_byte, !PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_is_nonzero(PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(ai, i)), aj + offset_byte, n_w_byte - offset_byte);
        }
        uint8_t pivot = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(ai, i);
        r8 &= PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_is_nonzero(pivot);
        pivot = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16_inv(pivot);
        offset_byte = (i + 1) >> 1;
        gf16v_mul_scalar(ai + offset_byte, pivot, n_w_byte - offset_byte);
        for (unsigned j = 0; j < h; j++) {
            if (i == j) {
                continue;
            }
            uint8_t *aj = mat + n_w_byte * j;
            gf16v_madd(aj + offset_byte, ai + offset_byte, PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(aj, i), n_w_byte - offset_byte);
        }
    }
    return r8;
}

static
unsigned gf16mat_solve_linear_eq_ref(uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned n) {
    uint8_t mat[64 * 33];
    unsigned n_byte = (n + 1) >> 1;
    for (unsigned i = 0; i < n; i++) {
        memcpy(mat + i * (n_byte + 1), inp_mat + i * n_byte, n_byte);
        mat[i * (n_byte + 1) + n_byte] = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_get_ele(c_terms, i);
    }
    unsigned r8 = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_gauss_elim(mat, n, n + 2);  /// XXX: this function is ``defined'' in blas.h
    for (unsigned i = 0; i < n; i++) {
        PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_set_ele(sol, i, mat[i * (n_byte + 1) + n_byte]);
    }
    return r8;
}

static inline void gf16mat_submat(uint8_t *mat2, unsigned w2, unsigned st, const uint8_t *mat, unsigned w, unsigned h) {
    unsigned n_byte_w1 = (w + 1) / 2;
    unsigned n_byte_w2 = (w2 + 1) / 2;
    unsigned st_2 = st / 2;
    for (unsigned i = 0; i < h; i++) {
        for (unsigned j = 0; j < n_byte_w2; j++) {
            mat2[i * n_byte_w2 + j] = mat[i * n_byte_w1 + st_2 + j];
        }
    }
}

unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_inv(uint8_t *inv_a, const uint8_t *a, unsigned H, uint8_t *buffer) {
    unsigned n_w_byte = (H + 1) / 2;

    uint8_t *aa = buffer;
    for (unsigned i = 0; i < H; i++) {
        uint8_t *ai = aa + i * 2 * n_w_byte;
        PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(ai, 2 * n_w_byte);
        gf256v_add(ai, a + i * n_w_byte, n_w_byte);
        PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16v_set_ele(ai + n_w_byte, i, 1);
    }
    unsigned r8 = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_gauss_elim(aa, H, 2 * H);  /// XXX: would 2*H fail if H is odd ???
    gf16mat_submat(inv_a, H, H, aa, 2 * H, H);
    return r8;
}
#else
/// @brief get an element from GF(256) vector .
///
/// @param[in]  a         - the input vector a.
/// @param[in]  i         - the index in the vector a.
/// @return  the value of the element.
///
uint8_t PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_get_ele(const uint8_t *a, unsigned i) {
    return a[i];
}

unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_is_zero(const uint8_t *a, unsigned _num_byte) {
    uint8_t r = 0;
    while ( _num_byte-- ) {
        r |= a[0];
        a++;
    }
    return (0 == r);
}

/// polynomial multplication
/// School boook
void PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_polymul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned _num) {
    PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(c, _num * 2 - 1);
    for (unsigned i = 0; i < _num; i++) {
        gf256v_madd(c + i, a, b[i], _num);
    }
}

static void gf256mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(c, n_A_vec_byte);
    for (unsigned i = 0; i < n_A_width; i++) {
        gf256v_madd(c, matA, b[i], n_A_vec_byte);
        matA += n_A_vec_byte;
    }
}

void PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_mul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned len_vec) {
    unsigned n_vec_byte = len_vec;
    for (unsigned k = 0; k < len_vec; k++) {
        PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero(c, n_vec_byte);
        const uint8_t *bk = b + n_vec_byte * k;
        for (unsigned i = 0; i < len_vec; i++) {
            gf256v_madd(c, a + n_vec_byte * i, bk[i], n_vec_byte);
        }
        c += n_vec_byte;
    }
}

static
unsigned gf256mat_gauss_elim_ref( uint8_t *mat, unsigned h, unsigned w ) {
    unsigned r8 = 1;

    for (unsigned i = 0; i < h; i++) {
        uint8_t *ai = mat + w * i;
        unsigned skip_len_align4 = i & ((unsigned)~0x3);

        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *aj = mat + w * j;
            gf256v_predicated_add( ai + skip_len_align4, !PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256_is_nonzero(ai[i]), aj + skip_len_align4, w - skip_len_align4 );
        }
        r8 &= PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256_is_nonzero(ai[i]);
        uint8_t pivot = ai[i];
        pivot = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256_inv( pivot );
        gf256v_mul_scalar( ai + skip_len_align4, pivot, w - skip_len_align4 );
        for (unsigned j = 0; j < h; j++) {
            if (i == j) {
                continue;
            }
            uint8_t *aj = mat + w * j;
            gf256v_madd( aj + skip_len_align4, ai + skip_len_align4, aj[i], w - skip_len_align4 );
        }
    }

    return r8;
}

static
unsigned gf256mat_solve_linear_eq_ref( uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned n ) {
    uint8_t mat[ 64 * 64 ];
    for (unsigned i = 0; i < n; i++) {
        memcpy( mat + i * (n + 1), inp_mat + i * n, n );
        mat[i * (n + 1) + n] = c_terms[i];
    }
    unsigned r8 = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_gauss_elim( mat, n, n + 1 );
    for (unsigned i = 0; i < n; i++) {
        sol[i] = mat[i * (n + 1) + n];
    }
    return r8;
}



static inline
void gf256mat_submat( uint8_t *mat2, unsigned w2, unsigned st, const uint8_t *mat, unsigned w, unsigned h ) {
    for (unsigned i = 0; i < h; i++) {
        for (unsigned j = 0; j < w2; j++) {
            mat2[i * w2 + j] = mat[i * w + st + j];
        }
    }
}


unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_inv( uint8_t *inv_a, const uint8_t *a, unsigned H, uint8_t *buffer ) {
    uint8_t *aa = buffer;
    for (unsigned i = 0; i < H; i++) {
        uint8_t *ai = aa + i * 2 * H;
        PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256v_set_zero( ai, 2 * H );
        gf256v_add( ai, a + i * H, H );
        ai[H + i] = 1;
    }
    unsigned r8 = PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_gauss_elim( aa, H, 2 * H );
    gf256mat_submat( inv_a, H, H, aa, 2 * H, H );
    return r8;
}

#endif




// choosing the implementations depends on the macros _BLAS_AVX2_ and _BLAS_SSE

#ifdef _USE_GF16
#define gf16mat_prod_impl             gf16mat_prod_ref
#define gf16mat_gauss_elim_impl       gf16mat_gauss_elim_ref
#define gf16mat_solve_linear_eq_impl  gf16mat_solve_linear_eq_ref



void PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_prod(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    gf16mat_prod_impl( c, matA, n_A_vec_byte, n_A_width, b);
}


unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_gauss_elim(uint8_t *mat, unsigned h, unsigned w) {
    return gf16mat_gauss_elim_impl( mat, h, w);
}

unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf16mat_solve_linear_eq( uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned n ) {
    return gf16mat_solve_linear_eq_impl( sol, inp_mat, c_terms, n );
}


#else
#define gf256mat_prod_impl            gf256mat_prod_ref
#define gf256mat_gauss_elim_impl      gf256mat_gauss_elim_ref
#define gf256mat_solve_linear_eq_impl gf256mat_solve_linear_eq_ref
void PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_prod(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    gf256mat_prod_impl( c, matA, n_A_vec_byte, n_A_width, b);
}

unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_gauss_elim( uint8_t *mat, unsigned h, unsigned w ) {
    return gf256mat_gauss_elim_impl( mat, h, w );
}


unsigned PQCLEAN_RAINBOWIACLASSIC_CLEAN_gf256mat_solve_linear_eq( uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned n ) {
    return gf256mat_solve_linear_eq_impl( sol, inp_mat, c_terms, n );
}

#endif

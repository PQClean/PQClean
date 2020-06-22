#ifndef COMMON_H
#define COMMON_H

int PQCLEAN_FRODOKEM1344SHAKE_OPT_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int PQCLEAN_FRODOKEM1344SHAKE_OPT_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_sample_n(uint16_t *s, size_t n);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_add(uint16_t *out, const uint16_t *a, const uint16_t *b);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_sub(uint16_t *out, const uint16_t *a, const uint16_t *b);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_key_encode(uint16_t *out, const uint16_t *in);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_key_decode(uint16_t *out, const uint16_t *in);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_pack(uint8_t *out, size_t outlen, const uint16_t *in, size_t inlen, uint8_t lsb);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_unpack(uint16_t *out, size_t outlen, const uint8_t *in, size_t inlen, uint8_t lsb);
int8_t PQCLEAN_FRODOKEM1344SHAKE_OPT_ct_verify(const uint16_t *a, const uint16_t *b, size_t len);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_ct_select(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t len, int8_t selector);
void PQCLEAN_FRODOKEM1344SHAKE_OPT_clear_bytes(uint8_t *mem, size_t n);
uint16_t PQCLEAN_FRODOKEM1344SHAKE_OPT_LE_TO_UINT16(uint16_t n);
uint16_t PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(uint16_t n);

#endif

#ifndef COMMON_H
#define COMMON_H

int PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_sample_n(uint16_t *s, size_t n);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_add(uint16_t *out, const uint16_t *a, const uint16_t *b);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_sub(uint16_t *out, const uint16_t *a, const uint16_t *b);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_key_encode(uint16_t *out, const uint16_t *in);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_key_decode(uint16_t *out, const uint16_t *in);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_pack(unsigned char *out, size_t outlen, const uint16_t *in, size_t inlen, unsigned char lsb);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_unpack(uint16_t *out, size_t outlen, const unsigned char *in, size_t inlen, unsigned char lsb);
void PQCLEAN_FRODOKEM640SHAKE_CLEAN_clear_bytes(uint8_t *mem, size_t n);

#endif

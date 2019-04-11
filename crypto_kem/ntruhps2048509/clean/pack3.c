#include "poly.h"

void PQCLEAN_NTRUHPS2048509_CLEAN_poly_S3_tobytes(unsigned char msg[NTRU_OWCPA_MSGBYTES], const poly *a)
{
  int i;
  unsigned char c;
  int j;

  for(i=0; i<NTRU_PACK_DEG/5; i++)
  {
    c =        a->coeffs[5*i+4] & 255;
    c = (3*c + a->coeffs[5*i+3]) & 255;
    c = (3*c + a->coeffs[5*i+2]) & 255;
    c = (3*c + a->coeffs[5*i+1]) & 255;
    c = (3*c + a->coeffs[5*i+0]) & 255;
    msg[i] = c;
  }
#if NTRU_PACK_DEG > (NTRU_PACK_DEG / 5) * 5  // if 5 does not divide NTRU_N-1
  i = NTRU_PACK_DEG/5;
  c = 0;
  for(j = NTRU_PACK_DEG - (5*i) - 1; j>=0; j--)
    c = (3*c + a->coeffs[5*i+j]) & 255;
  msg[i] = c;
#endif
}

void PQCLEAN_NTRUHPS2048509_CLEAN_poly_S3_frombytes(poly *r, const unsigned char msg[NTRU_OWCPA_MSGBYTES])
{
  int i;
  unsigned char c;
  int j;

  for(i=0; i<NTRU_PACK_DEG/5; i++)
  {
    c = msg[i];
    r->coeffs[5*i+0] = PQCLEAN_NTRUHPS2048509_CLEAN_mod3(c);
    r->coeffs[5*i+1] = PQCLEAN_NTRUHPS2048509_CLEAN_mod3(c * 171 >> 9);  // this is division by 3
    r->coeffs[5*i+2] = PQCLEAN_NTRUHPS2048509_CLEAN_mod3(c * 57 >> 9);  // division by 3^2
    r->coeffs[5*i+3] = PQCLEAN_NTRUHPS2048509_CLEAN_mod3(c * 19 >> 9);  // division by 3^3
    r->coeffs[5*i+4] = PQCLEAN_NTRUHPS2048509_CLEAN_mod3(c * 203 >> 14);  // etc.
  }
#if NTRU_PACK_DEG > (NTRU_PACK_DEG / 5) * 5  // if 5 does not divide NTRU_N-1
  i = NTRU_PACK_DEG/5;
  c = msg[i];
  for(j=0; (5*i+j)<NTRU_PACK_DEG; j++)
  {
    r->coeffs[5*i+j] = PQCLEAN_NTRUHPS2048509_CLEAN_mod3(c);
    c = c * 171 >> 9;
  }
#endif
  r->coeffs[NTRU_N-1] = 0;
}


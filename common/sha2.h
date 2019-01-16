#ifndef SHA2_H
#define SHA2_H

int sha384(unsigned char *out, const unsigned char *in,
           unsigned long long inlen);
int sha512(unsigned char *out, const unsigned char *in,
           unsigned long long inlen);

#endif

#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H
#include <stdint.h>

#ifdef _WIN32
#include <CRTDEFS.H>
#else
#include <unistd.h>
#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC int randombytes(uint8_t *buf, size_t n);

#undef EXTERNC

#endif

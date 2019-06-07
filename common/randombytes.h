#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H
#include <stdint.h>

#ifdef _WIN32
#include <CRTDEFS.H>
#else
#include <unistd.h>
#endif

int randombytes(uint8_t *buf, size_t n);

#endif

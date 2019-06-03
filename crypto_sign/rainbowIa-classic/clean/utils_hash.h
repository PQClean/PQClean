/// @file utils_hash.h
/// @brief the interface for adapting hash functions.
///
///
#ifndef _UTILS_HASH_H_
#define _UTILS_HASH_H_

// for the definition of _HASH_LEN.
#include "hash_len_config.h"

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_hash_msg( unsigned char *digest, unsigned len_digest, const unsigned char *m, unsigned long long mlen );



#endif // _UTILS_HASH_H_


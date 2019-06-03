/// @file rainbow_config.h
/// @brief Defining the parameters of the Rainbow and the corresponding constants.
///
/// Defining one of the 3 parameter _RAINBOW16_32_32_32 , _RAINBOW256_68_36_36 , or _RAINBOW256_92_48_48
/// for (GF16,32,32,32) (GF256,68,36,36) (GF256,92,48,48) in this file.
///
///

#ifndef _H_RAINBOW_CONFIG_H_
#define _H_RAINBOW_CONFIG_H_


// TODO: refactor this

/// the defined parameter
#if (!defined(_RAINBOW16_32_32_32))&&(!defined(_RAINBOW256_68_36_36))&&(!defined(_RAINBOW256_92_48_48))
#define _RAINBOW16_32_32_32
//#define _RAINBOW256_68_36_36
//#define _RAINBOW256_92_48_48
#endif


#if defined _RAINBOW16_32_32_32
#define _USE_GF16
#define _GFSIZE 16
#define _V1 32
#define _O1 32
#define _O2 32
#define _HASH_LEN 32

#elif defined _RAINBOW256_68_36_36
#define _GFSIZE 256
#define _V1 68
#define _O1 36
#define _O2 36
#define _HASH_LEN 48

#elif defined _RAINBOW256_92_48_48
#define _GFSIZE 256
#define _V1 92
#define _O1 48
#define _O2 48
#define _HASH_LEN 64

#else
error here.
#endif


#define _V2 ((_V1)+(_O1))

#define STR1(x) #x
#define THE_NAME(gf,v1,o1,o2) "RAINBOW(" STR1(gf) "," STR1(v1) "," STR1(o1) "," STR1(o2) ")"
#define _S_NAME THE_NAME(_GFSIZE,_V1,_O1,_O2)


/// size of N, in # of gf elements.
#define _PUB_N  (_V1+_O1+_O2)

/// size of M, in # gf elements.
#define _PUB_M  (_O1+_O2)


/// size of variables, in # bytes.


#ifdef _USE_GF16
// GF16
#define _V1_BYTE (_V1/2)
#define _V2_BYTE (_V2/2)
#define _O1_BYTE (_O1/2)
#define _O2_BYTE (_O2/2)
#define _PUB_N_BYTE  (_PUB_N/2)
#define _PUB_M_BYTE  (_PUB_M/2)

#else
// GF256
#define _V1_BYTE (_V1)
#define _V2_BYTE (_V2)
#define _O1_BYTE (_O1)
#define _O2_BYTE (_O2)
#define _PUB_N_BYTE  (_PUB_N)
#define _PUB_M_BYTE  (_PUB_M)

#endif


/// length of seed for public key, in # bytes
#define LEN_PKSEED 32

/// length of seed for secret key, in # bytes
#define LEN_SKSEED 32

/// length of salt for a signature, in # bytes
#define _SALT_BYTE 16

/// length of a signature
#define _SIGNATURE_BYTE (_PUB_N_BYTE + _SALT_BYTE )

#endif //  _H_RAINBOW_CONFIG_H_

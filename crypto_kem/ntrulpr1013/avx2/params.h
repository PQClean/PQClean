#ifndef params_H
#define params_H
#include "crypto_core_multsntrup1013.h"
#include "crypto_decode_1013x2393.h"
#include "crypto_decode_1013x3.h"
#include "crypto_decode_1013xint16.h"
#include "crypto_decode_1013xint32.h"
#include "crypto_decode_256x16.h"
#include "crypto_decode_256x2.h"
#include "crypto_encode_1013x2393.h"
#include "crypto_encode_1013x2393round.h"
#include "crypto_encode_1013x3.h"
#include "crypto_encode_1013xint16.h"
#include "crypto_encode_256x16.h"
#include "crypto_encode_256x2.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1583.h"


#define p 1013
#define q 7177
#define w 392
#define tau0 3367
#define tau1 73
#define tau2 3143
#define tau3 449
#define I 256

#define ppadsort 1024

#define q18 37 /* round(2^18/q) */
#define q27 18701 /* round(2^27/q) */
#define q31 299217 /* floor(2^31/q) */

#define crypto_verify_clen PQCLEAN_NTRULPR1013_AVX2_crypto_verify_1583

#define Rounded_bytes PQCLEAN_NTRULPR1013_AVX2_crypto_decode_1013x2393_STRBYTES
#define Rounded_decode PQCLEAN_NTRULPR1013_AVX2_crypto_decode_1013x2393

#define Round_and_encode PQCLEAN_NTRULPR1013_AVX2_crypto_encode_1013x2393round

#define Small_bytes PQCLEAN_NTRULPR1013_AVX2_crypto_encode_1013x3_STRBYTES
#define Small_encode PQCLEAN_NTRULPR1013_AVX2_crypto_encode_1013x3
#define Small_decode PQCLEAN_NTRULPR1013_AVX2_crypto_decode_1013x3

#define Top_bytes PQCLEAN_NTRULPR1013_AVX2_crypto_encode_256x16_STRBYTES
#define Top_encode PQCLEAN_NTRULPR1013_AVX2_crypto_encode_256x16
#define Top_decode PQCLEAN_NTRULPR1013_AVX2_crypto_decode_256x16

#define Inputs_bytes PQCLEAN_NTRULPR1013_AVX2_crypto_encode_256x2_STRBYTES
#define Inputs_encode PQCLEAN_NTRULPR1013_AVX2_crypto_encode_256x2
#define Inputs_decode PQCLEAN_NTRULPR1013_AVX2_crypto_decode_256x2

#define crypto_decode_pxint32 PQCLEAN_NTRULPR1013_AVX2_crypto_decode_1013xint32

#define crypto_decode_pxint16 PQCLEAN_NTRULPR1013_AVX2_crypto_decode_1013xint16

#define crypto_encode_pxint16 PQCLEAN_NTRULPR1013_AVX2_crypto_encode_1013xint16

#define crypto_core_mult PQCLEAN_NTRULPR1013_AVX2_crypto_core_multsntrup1013

#endif

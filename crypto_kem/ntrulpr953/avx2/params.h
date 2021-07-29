#ifndef params_H
#define params_H
#include "crypto_core_multsntrup953.h"
#include "crypto_decode_256x16.h"
#include "crypto_decode_256x2.h"
#include "crypto_decode_953x2115.h"
#include "crypto_decode_953x3.h"
#include "crypto_decode_953xint16.h"
#include "crypto_decode_953xint32.h"
#include "crypto_encode_256x16.h"
#include "crypto_encode_256x2.h"
#include "crypto_encode_953x2115.h"
#include "crypto_encode_953x2115round.h"
#include "crypto_encode_953x3.h"
#include "crypto_encode_953xint16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1477.h"


#define p 953
#define q 6343
#define w 345
#define tau0 2997
#define tau1 82
#define tau2 2798
#define tau3 400
#define I 256

#define ppadsort 960

#define q18 41 /* round(2^18/q) */
#define q27 21160 /* round(2^27/q) */
#define q31 338559 /* floor(2^31/q) */

#define crypto_verify_clen PQCLEAN_NTRULPR953_AVX2_crypto_verify_1477

#define Rounded_bytes PQCLEAN_NTRULPR953_AVX2_crypto_decode_953x2115_STRBYTES
#define Rounded_decode PQCLEAN_NTRULPR953_AVX2_crypto_decode_953x2115

#define Round_and_encode PQCLEAN_NTRULPR953_AVX2_crypto_encode_953x2115round

#define Small_bytes PQCLEAN_NTRULPR953_AVX2_crypto_encode_953x3_STRBYTES
#define Small_encode PQCLEAN_NTRULPR953_AVX2_crypto_encode_953x3
#define Small_decode PQCLEAN_NTRULPR953_AVX2_crypto_decode_953x3

#define Top_bytes PQCLEAN_NTRULPR953_AVX2_crypto_encode_256x16_STRBYTES
#define Top_encode PQCLEAN_NTRULPR953_AVX2_crypto_encode_256x16
#define Top_decode PQCLEAN_NTRULPR953_AVX2_crypto_decode_256x16

#define Inputs_bytes PQCLEAN_NTRULPR953_AVX2_crypto_encode_256x2_STRBYTES
#define Inputs_encode PQCLEAN_NTRULPR953_AVX2_crypto_encode_256x2
#define Inputs_decode PQCLEAN_NTRULPR953_AVX2_crypto_decode_256x2

#define crypto_decode_pxint32 PQCLEAN_NTRULPR953_AVX2_crypto_decode_953xint32

#define crypto_decode_pxint16 PQCLEAN_NTRULPR953_AVX2_crypto_decode_953xint16

#define crypto_encode_pxint16 PQCLEAN_NTRULPR953_AVX2_crypto_encode_953xint16

#define crypto_core_mult PQCLEAN_NTRULPR953_AVX2_crypto_core_multsntrup953

#endif

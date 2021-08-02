#ifndef params_H
#define params_H
#include "crypto_core_inv3sntrup953.h"
#include "crypto_core_invsntrup953.h"
#include "crypto_core_mult3sntrup953.h"
#include "crypto_core_multsntrup953.h"
#include "crypto_core_scale3sntrup953.h"
#include "crypto_core_weightsntrup953.h"
#include "crypto_core_wforcesntrup953.h"
#include "crypto_decode_953x2115.h"
#include "crypto_decode_953x3.h"
#include "crypto_decode_953x6343.h"
#include "crypto_decode_953xint16.h"
#include "crypto_decode_953xint32.h"
#include "crypto_encode_953x2115.h"
#include "crypto_encode_953x2115round.h"
#include "crypto_encode_953x3.h"
#include "crypto_encode_953x6343.h"
#include "crypto_encode_953xfreeze3.h"
#include "crypto_encode_953xint16.h"
#include "crypto_encode_int16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1349.h"


#define q31 338559 /* floor(2^31/q) */
#define q27 21160 /* closest integer to 2^27/q */
#define q18 41 /* closest integer to 2^18/q */
#define q14 3 /* closest integer to 2^14/q */
#define p 953
#define q 6343
#define w 396

#define ppadsort 960

#define crypto_verify_clen PQCLEAN_SNTRUP953_CLEAN_crypto_verify_1349

#define Rq_bytes PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x6343_STRBYTES
#define Rq_encode PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x6343
#define Rq_decode PQCLEAN_SNTRUP953_CLEAN_crypto_decode_953x6343

#define Rounded_bytes PQCLEAN_SNTRUP953_CLEAN_crypto_decode_953x2115_STRBYTES
#define Rounded_decode PQCLEAN_SNTRUP953_CLEAN_crypto_decode_953x2115

#define Round_and_encode PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x2115round

#define Small_bytes PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x3_STRBYTES
#define Small_encode PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x3
#define Small_decode PQCLEAN_SNTRUP953_CLEAN_crypto_decode_953x3

#define crypto_encode_pxfreeze3 PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953xfreeze3

#define crypto_decode_pxint32 PQCLEAN_SNTRUP953_CLEAN_crypto_decode_953xint32

#define crypto_decode_pxint16 PQCLEAN_SNTRUP953_CLEAN_crypto_decode_953xint16

#define crypto_encode_pxint16 PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953xint16

#define crypto_core_wforce PQCLEAN_SNTRUP953_CLEAN_crypto_core_wforcesntrup953

#define crypto_core_scale3 PQCLEAN_SNTRUP953_CLEAN_crypto_core_scale3sntrup953

#define crypto_core_inv PQCLEAN_SNTRUP953_CLEAN_crypto_core_invsntrup953

#define crypto_core_inv3 PQCLEAN_SNTRUP953_CLEAN_crypto_core_inv3sntrup953

#define crypto_core_mult3 PQCLEAN_SNTRUP953_CLEAN_crypto_core_mult3sntrup953

#define crypto_core_mult PQCLEAN_SNTRUP953_CLEAN_crypto_core_multsntrup953

#endif

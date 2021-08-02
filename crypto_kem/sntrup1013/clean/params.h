#ifndef params_H
#define params_H
#include "crypto_core_inv3sntrup1013.h"
#include "crypto_core_invsntrup1013.h"
#include "crypto_core_mult3sntrup1013.h"
#include "crypto_core_multsntrup1013.h"
#include "crypto_core_scale3sntrup1013.h"
#include "crypto_core_weightsntrup1013.h"
#include "crypto_core_wforcesntrup1013.h"
#include "crypto_decode_1013x2393.h"
#include "crypto_decode_1013x3.h"
#include "crypto_decode_1013x7177.h"
#include "crypto_decode_1013xint16.h"
#include "crypto_decode_1013xint32.h"
#include "crypto_encode_1013x2393.h"
#include "crypto_encode_1013x2393round.h"
#include "crypto_encode_1013x3.h"
#include "crypto_encode_1013x7177.h"
#include "crypto_encode_1013xfreeze3.h"
#include "crypto_encode_1013xint16.h"
#include "crypto_encode_int16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1455.h"


#define q31 299217 /* floor(2^31/q) */
#define q27 18701 /* closest integer to 2^27/q */
#define q18 37 /* closest integer to 2^18/q */
#define q14 2 /* closest integer to 2^14/q */
#define p 1013
#define q 7177
#define w 448

#define ppadsort 1024

#define crypto_verify_clen PQCLEAN_SNTRUP1013_CLEAN_crypto_verify_1455

#define Rq_bytes PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x7177_STRBYTES
#define Rq_encode PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x7177
#define Rq_decode PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013x7177

#define Rounded_bytes PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013x2393_STRBYTES
#define Rounded_decode PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013x2393

#define Round_and_encode PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x2393round

#define Small_bytes PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x3_STRBYTES
#define Small_encode PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x3
#define Small_decode PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013x3

#define crypto_encode_pxfreeze3 PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013xfreeze3

#define crypto_decode_pxint32 PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013xint32

#define crypto_decode_pxint16 PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013xint16

#define crypto_encode_pxint16 PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013xint16

#define crypto_core_wforce PQCLEAN_SNTRUP1013_CLEAN_crypto_core_wforcesntrup1013

#define crypto_core_scale3 PQCLEAN_SNTRUP1013_CLEAN_crypto_core_scale3sntrup1013

#define crypto_core_inv PQCLEAN_SNTRUP1013_CLEAN_crypto_core_invsntrup1013

#define crypto_core_inv3 PQCLEAN_SNTRUP1013_CLEAN_crypto_core_inv3sntrup1013

#define crypto_core_mult3 PQCLEAN_SNTRUP1013_CLEAN_crypto_core_mult3sntrup1013

#define crypto_core_mult PQCLEAN_SNTRUP1013_CLEAN_crypto_core_multsntrup1013

#endif

/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
/*
*
*  $File$
*
*  $Author$
*
*  $Date$
*
*  $Rev$
*
***********************************************************************************************************************/
#ifndef FSMSW_FALCON_CODEC_H
#define FSMSW_FALCON_CODEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
/* Number of bits for key elements, indexed by logn (1 to 10). This is at most 8 bits for all degrees, but some degrees
 * may have shorter elements. */
extern const uint8 FsmSw_Falcon_max_small_fg_bits[11];
extern const uint8 FsmSw_Falcon_max_big_FG_bits[11];

/* Maximum size, in bits, of elements in a signature, indexed by logn (1 to 10). The size includes the sign bit. */
extern const uint8 FsmSw_Falcon_max_sig_bits[11];

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/* Encoding functions take as parameters an output buffer (out) with a given maximum length (max_out_len); returned
 * value is the actual number of bytes which have been written. If the output buffer is not large enough, then 0 is
 * returned (some bytes may have been written to the buffer). If 'out' is NULL, then 'max_out_len' is ignored; instead,
 * the function computes and returns the actual required output length (in bytes).
 *
 * Decoding functions take as parameters an input buffer (in) with its maximum length (max_in_len); returned value is
 * the actual number of bytes that have been read from the buffer. If the provided length is too short, then 0 is
 * returned.
 *
 * Values to encode or decode are vectors of integers, with N = 2^logn elements.
 *
 * Three encoding formats are defined:
 *
 *   - modq: sequence of values modulo 12289, each encoded over exactly 14 bits. The encoder and decoder verify that
 *           integers are within the valid range (0..12288). Values are arrays of uint16.
 *
 *   - trim: sequence of signed integers, a specified number of bits each. The number of bits is provided as parameter
 *           and includes the sign bit. Each integer x must be such that |x| < 2^(bits-1) (which means that the
 *           -2^(bits-1) value is forbidden); encode and decode functions check that property. Values are arrays of
 *           sint16 or sint8, corresponding to names 'trim_i16' and 'trim_i8', respectively.
 *
 *   - comp: variable-length encoding for signed integers; each integer uses a minimum of 9 bits, possibly more. This
 *           is normally used only for signatures. */

uint32 FsmSw_Falcon_ModqEncode(void *out, uint32 max_out_len, const uint16 *x, uint32 logn);
uint32 FsmSw_Falcon_ModqDecode(uint16 *x, uint32 logn, const void *in, uint32 max_in_len);
uint32 FsmSw_Falcon_TrimI8Encode(void *out, uint32 max_out_len, const sint8 *x, uint32 logn, uint32 bits);
uint32 FsmSw_Falcon_TrimI8Decode(sint8 *x, uint32 logn, uint32 bits, const void *in, uint32 max_in_len);
uint32 FsmSw_Falcon_CompEncode(void *out, uint32 max_out_len, const sint16 *x, uint32 logn);
uint32 FsmSw_Falcon_CompDecode(sint16 *x, uint32 logn, const void *in, uint32 max_in_len);

#endif /* FSMSW_FALCON_CODEC_H */

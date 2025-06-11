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
#ifndef FSMSW_FALCON_COMMON_H
#define FSMSW_FALCON_COMMON_H

/* IMPORTANT API RULES
 * -------------------
 *
 * This API has some non-trivial usage rules:
 *  - All public functions (i.e. the non-static ones) must be referenced with the FsmSw_Falcon_ macro (e.g.
 *    FsmSw_Falcon_VerifyRaw for the verify_raw() function). That macro adds a prefix to the name, which is
 *    configurable with the FALCON_PREFIX macro. This allows compiling the code into a specific "namespace" and
 *    potentially including several versions of this code into a single application (e.g. to have an AVX2 and a
 *    non-AVX2 variants and select the one to use at runtime based on availability of AVX2 opcodes).
 *  - Functions that need temporary buffers expects them as a final tmp[] array of type uint8*, with a size which is
 *    documented for each function. However, most have some alignment requirements, because they will use the array to
 *    store 16-bit, 32-bit or 64-bit values (e.g. uint64 or double). The caller must ensure proper alignment. What
 *    happens on unaligned access depends on the underlying architecture, ranging from a slight time penalty to
 *    immediate termination of the process.
 *  - Some functions rely on specific rounding rules and precision for floating-point numbers. On some systems (in
 *    particular 32-bit x86 with the 387 FPU), this requires setting an hardware control word. The caller MUST use
 *    set_fpu_cw() to ensure proper precision:
 *      oldcw = set_fpu_cw(2);
 *      FsmSw_Falcon_Sign_Dyn(...);
 *      set_fpu_cw(oldcw);
 *
 *    On systems where the native floating-point precision is already proper, or integer-based emulation is used, the
 *    set_fpu_cw() function does nothing, so it can be called systematically. */

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Fips202.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* API is defined to be easily replaced with the fips202.h API defined as part of PQClean. */
#define inner_shake256_context shake256incctx

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* Normally we should declare the 'fpr' type to be a struct or union around the internal 64-bit value; however, we want
 * to use the direct 64-bit integer type to enable a lighter call convention on ARM platforms. This means that direct
 * (invalid) use of operators such as '*' or '+' will not be caught by the compiler. We rely on the "normal"
 * (non-emulated) code to detect such instances. */
typedef uint64 fpr;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Falcon_HashToPointVartime(inner_shake256_context *sc, uint16 *x, uint32 logn);
void FsmSw_Falcon_HashToPointCt(inner_shake256_context *sc, uint16 *x, uint32 logn, uint8 *tmp);
sint32 FsmSw_Falcon_IsShort(const sint16 *s1, const sint16 *s2, uint32 logn);
sint32 FsmSw_Falcon_IsShortHalf(uint32 sqn, const sint16 *s2, uint32 logn);

#endif /* FSMSW_FALCON_COMMON_H */

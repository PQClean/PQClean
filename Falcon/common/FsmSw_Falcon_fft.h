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
#ifndef FSMSW_FALCON_FFT_H
#define FSMSW_FALCON_FFT_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Falcon_fpr.h"
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

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Falcon_FFT(fpr *f, uint32 logn);

void FsmSw_Falcon_IFFT(fpr *f, uint32 logn);

void FsmSw_Falcon_Poly_Add(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_Sub(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_Neg(fpr *a, uint32 logn);

void FsmSw_Falcon_Poly_AdjFFT(fpr *a, uint32 logn);

void FsmSw_Falcon_Poly_MulFFT(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_MuladjFFT(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_MulselfadjFFT(fpr *a, uint32 logn);

void FsmSw_Falcon_Poly_Mulconst(fpr *a, fpr x, uint32 logn);

void FsmSw_Falcon_Poly_Invnorm2FFT(fpr *d, const fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_Add_MuladjFFT(fpr *d, const fpr *F, const fpr *G, const fpr *f, const fpr *g, uint32 logn);

void FsmSw_Falcon_Poly_Mul_AutoadjFFT(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_Div_AutoadjFFT(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_Poly_LdlFFT(const fpr *g00, fpr *g01, fpr *g11, uint32 logn);

void FsmSw_Falcon_Poly_LdlMvFFT(fpr *d11, fpr *l10, const fpr *g00, const fpr *g01, const fpr *g11, uint32 logn);

void FsmSw_Falcon_Poly_SplitFFT(fpr *f0, fpr *f1, const fpr *f, uint32 logn);

void FsmSw_Falcon_Poly_MergeFFT(fpr *f, const fpr *f0, const fpr *f1, uint32 logn);

#endif /* FSMSW_FALCON_FFT_H */

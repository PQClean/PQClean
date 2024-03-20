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
#include "Platform_Types.h"
#include "FsmSw_Falcon_fpr.h"

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

void FsmSw_Falcon_iFFT(fpr *f, uint32 logn);

void FsmSw_Falcon_poly_add(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_sub(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_neg(fpr *a, uint32 logn);

void FsmSw_Falcon_poly_adj_fft(fpr *a, uint32 logn);

void FsmSw_Falcon_poly_mul_fft(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_muladj_fft(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_mulselfadj_fft(fpr *a, uint32 logn);

void FsmSw_Falcon_poly_mulconst(fpr *a, fpr x, uint32 logn);

void FsmSw_Falcon_poly_invnorm2_fft(fpr *d, const fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_add_muladj_fft(fpr *d, const fpr *F, const fpr *G, const fpr *f, const fpr *g, uint32 logn);

void FsmSw_Falcon_poly_mul_autoadj_fft(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_div_autoadj_fft(fpr *a, const fpr *b, uint32 logn);

void FsmSw_Falcon_poly_LDL_fft(const fpr *g00, fpr *g01, fpr *g11, uint32 logn);

void FsmSw_Falcon_poly_LDLmv_fft(fpr *d11, fpr *l10, const fpr *g00, const fpr *g01, const fpr *g11, uint32 logn);

void FsmSw_Falcon_poly_split_fft(fpr *f0, fpr *f1, const fpr *f, uint32 logn);

void FsmSw_Falcon_poly_merge_fft(fpr *f, const fpr *f0, const fpr *f1, uint32 logn);

#endif /* FSMSW_FALCON_FFT_H */

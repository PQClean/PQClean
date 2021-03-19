/**
 *
 * Reference ISO-C11 Implementation of LEDAcrypt.
 *
 * @version 3.0 (May 2020)
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#ifndef BF_DECODING_H
#define BF_DECODING_H

#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#define  ITERATIONS_MAX   (2)

int bf_decoding(DIGIT err[],
                const POSITION_T HtrPosOnes[N0][V],
                DIGIT privateSyndrome[]       //  1 polynomial  -- param. in/out
               );
/* thresholds and tbars for imaxin = 0imaxout = 2 CCA ledadecoder */
#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define T_BAR 10
#define B0 39
#endif

#if (CATEGORY == 1) && (N0 == 3) && (DFR_SL_LEVEL == 0)
#define T_BAR 9
#define B0 43
#endif

#if (CATEGORY == 1) && (N0 == 4) && (DFR_SL_LEVEL == 0)
#define T_BAR 8
#define B0 46
#endif

#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define T_BAR 11
#define B0 38
#endif

#if (CATEGORY == 1) && (N0 == 3) && (DFR_SL_LEVEL == 1)
#define T_BAR 10
#define B0 43
#endif

#if (CATEGORY == 1) && (N0 == 4) && (DFR_SL_LEVEL == 1)
#define T_BAR 9
#define B0 45
#endif

#if (CATEGORY == 3) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define T_BAR 13
#define B0 56
#endif

#if (CATEGORY == 3) && (N0 == 3) && (DFR_SL_LEVEL == 0)
#define T_BAR 11
#define B0 63
#endif

#if (CATEGORY == 3) && (N0 == 4) && (DFR_SL_LEVEL == 0)
#define T_BAR 11
#define B0 67
#endif

#if (CATEGORY == 3) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define T_BAR 15
#define B0 55
#endif

#if (CATEGORY == 3) && (N0 == 3) && (DFR_SL_LEVEL == 1)
#define T_BAR 13
#define B0 62
#endif

#if (CATEGORY == 3) && (N0 == 4) && (DFR_SL_LEVEL == 1)
#define T_BAR 12
#define B0 66
#endif

#if (CATEGORY == 5) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define T_BAR 17
#define B0 74
#endif

#if (CATEGORY == 5) && (N0 == 3) && (DFR_SL_LEVEL == 0)
#define T_BAR 14
#define B0 82
#endif

#if (CATEGORY == 5) && (N0 == 4) && (DFR_SL_LEVEL == 0)
#define T_BAR 13
#define B0 88
#endif

#if (CATEGORY == 5) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define T_BAR 18
#define B0 72
#endif

#if (CATEGORY == 5) && (N0 == 3) && (DFR_SL_LEVEL == 1)
#define T_BAR 16
#define B0 81
#endif

#if (CATEGORY == 5) && (N0 == 4) && (DFR_SL_LEVEL == 1)
#define T_BAR 15
#define B0 85
#endif

#endif
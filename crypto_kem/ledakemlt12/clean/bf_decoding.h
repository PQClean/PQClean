/**
 *
 * <bf_decoding.h>
 *
 * @version 2.0 (March 2019)
 *
 * Reference ISO-C11 Implementation of the LEDAcrypt KEM-LT cipher using GCC built-ins.
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


#pragma once
#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#define  ITERATIONS_MAX   (2)


int bf_decoding(DIGIT err[],
                const POSITION_T HtrPosOnes[N0][DV],
                const POSITION_T QtrPosOnes[N0][M], // N0 vectors containing exp.s of Qtr ones
                DIGIT privateSyndrome[]       //  1 polynomial  -- param. in/out
               );

/*  Definitions for DFR level 2^-64 */
#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define B0 44
#define T_BAR 4
#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define B0 64
#define T_BAR 5
#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define B0 89
#define T_BAR 6
#endif

/*  Definitions for DFR level 2^-SL */
#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define B0 43
#define T_BAR 4
#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define B0 64
#define T_BAR 5
#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define B0 88
#define T_BAR 6
#endif



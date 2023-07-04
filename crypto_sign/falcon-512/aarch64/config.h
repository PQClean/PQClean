/*
 * Config
 *
 * =============================================================================
 * Copyright (c) 2021 by Cryptographic Engineering Research Group (CERG)
 * ECE Department, George Mason University
 * Fairfax, VA, U.S.A.
 * Author: Duc Tri Nguyen
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * =============================================================================
 * @author   Duc Tri Nguyen <dnguye69@gmu.edu>
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "params.h"

/*
 * By default, benchmark using APPLE_M1 is disable
 */
#ifndef APPLE_M1
#define APPLE_M1 0
#endif

/*
 * By default, complex instruction on ARMv8.2 is auto enable on M1
 * otherwise it is disable
 */
#if APPLE_M1 == 1
#define COMPLEX 1
#else
#define COMPLEX 0
#endif

/*
 * By default, FMA is disabled due to rounding error between (FADD, FMUL) and FMA
 */
#define FMA 0

/*
 * By default, NEON is little Edian, either 0 or 1 will must pass the test
 * FALCON_LE: Little Edian flag
 */
#define FALCON_LE 0

/*
 * By default, FPEMU is 0 on NEON implementation
 * FALCON_FPEMU: Floating-point emulator flag
 */
#define FALCON_FPEMU 0
#if FALCON_FPEMU != 0
#error "FPEMU is not support in NEON implementation"
#endif

#endif

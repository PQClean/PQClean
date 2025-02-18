/*
 * SPDX-License-Identifer: Apache-2.0
 * Copyright (c) 2023 by Cryptographic Engineering Research Group (CERG) ECE Department, George Mason University Fairfax, VA, U.S.A.
 * Author: Duc Tri Nguyen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author   Duc Tri Nguyen <dnguye69@gmu.edu>, <cothannguyen@gmail.com>
 *
 * Copyright Thomas Pornin <thomas.pornin@nccgroup.com>.
 *
 * It has been reported that patent US7308097B2 may be applicable to parts
 * of Falcon. William Whyte, one of the designers of Falcon and also
 * representative of OnBoard Security (current owner of the said patent),
 * has pledged, as part of the IP statements submitted to the NIST for the
 * PQC project, that in the event of Falcon being selected for
 * standardization, a worldwide non-exclusive license to the patent will be
 * granted for the purpose of implementing the standard "without
 * compensation and under reasonable terms and conditions that are
 * demonstrably free of any unfair discrimination".
*/

#ifndef POLY_H
#define POLY_H

#include "inner.h"
#include "params.h"

typedef enum ntt_domain {
    NTT_NONE = 0,
    NTT_MONT = 1,
    NTT_MONT_INV = 2,
} ntt_domain_t;

typedef enum invntt_domain {
    INVNTT_NONE = 0,
    INVNTT_NINV = 1,
} invntt_domain_t;

void PQCLEAN_FALCONPADDED512_AARCH64_poly_ntt(int16_t a[FALCON_N], ntt_domain_t mont);

void PQCLEAN_FALCONPADDED512_AARCH64_poly_invntt(int16_t a[FALCON_N], invntt_domain_t ninv);

void PQCLEAN_FALCONPADDED512_AARCH64_poly_int8_to_int16(int16_t out[FALCON_N], const int8_t in[FALCON_N]);

void PQCLEAN_FALCONPADDED512_AARCH64_poly_div_12289(int16_t f[FALCON_N], const int16_t g[FALCON_N]);

void PQCLEAN_FALCONPADDED512_AARCH64_poly_convert_to_unsigned(int16_t f[FALCON_N]);

uint16_t PQCLEAN_FALCONPADDED512_AARCH64_poly_compare_with_zero(int16_t f[FALCON_N]);

void PQCLEAN_FALCONPADDED512_AARCH64_poly_montmul_ntt(int16_t f[FALCON_N], const int16_t g[FALCON_N]);

void PQCLEAN_FALCONPADDED512_AARCH64_poly_sub_barrett(int16_t f[FALCON_N], const int16_t g[FALCON_N], const int16_t s[FALCON_N]);

int PQCLEAN_FALCONPADDED512_AARCH64_poly_int16_to_int8(int8_t G[FALCON_N], const int16_t t[FALCON_N]);

int PQCLEAN_FALCONPADDED512_AARCH64_poly_check_bound_int8(const int8_t t[FALCON_N],
        const int8_t low, const int8_t high);

int PQCLEAN_FALCONPADDED512_AARCH64_poly_check_bound_int16(const int16_t t[FALCON_N],
        const int16_t low, const int16_t high);

#endif

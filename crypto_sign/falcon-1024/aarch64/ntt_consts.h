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

#ifndef NTT_CONSTS
#define NTT_CONSTS

#include <stdint.h>

extern const int16_t PQCLEAN_FALCON1024_AARCH64_qmvq[8];

/*
 * Table for NTT, binary case:
 * where g = 7 (it is a 2048-th primitive root of 1 modulo q)
 */
extern const int16_t PQCLEAN_FALCON1024_AARCH64_ntt_br[];
extern const int16_t PQCLEAN_FALCON1024_AARCH64_ntt_qinv_br[];

/*
 * Table for inverse NTT
 * Since g = 7, 1/g = 8778 mod 12289.
 */

extern const int16_t PQCLEAN_FALCON1024_AARCH64_invntt_br[];
extern const int16_t PQCLEAN_FALCON1024_AARCH64_invntt_qinv_br[];

#endif

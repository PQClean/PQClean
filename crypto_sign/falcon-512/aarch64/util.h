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

#ifndef UTIL_H
#define UTIL_H

#define poly_small_to_fp PQCLEAN_FALCON512_AARCH64_smallints_to_fpr

void PQCLEAN_FALCON512_AARCH64_smallints_to_fpr(fpr *r, const int8_t *t, unsigned logn);

#endif

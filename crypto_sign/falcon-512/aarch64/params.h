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

#ifndef PARAMS_H
#define PARAMS_H

#define FALCON_LOGN 9

#define FALCON_N (1 << FALCON_LOGN)
#define FALCON_Q 12289
#define FALCON_QINV (-12287) // pow(12289, -1, pow(2, 16)) - pow(2, 16)
#define FALCON_V 5461        // Barrett reduction
#define FALCON_MONT 4091     // pow(2, 16, 12289)
#define FALCON_MONT_BR 10908 // (4091 << 16)//q//2

#define FALCON_NINV_MONT 128    // pow(512, -1, 12289) * pow(2, 16, 12289)
#define FALCON_NINV_MONT_BR 341 // (128 << 16) //q // 2
#define FALCON_LOG2_NINV_MONT 7

#endif

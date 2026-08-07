/*
 * SPDX-License-Identifier: MIT
 * Copyright: National Institute of Standards and Technology; The software developed by NIST employees is not subject to copyright protection within the United States.
 * Copyright: 2017-2019  Falcon Project
 * Copyright: PQClean Project
 *
 * NIST-developed software is provided by NIST as a public service. You may use, copy, and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify, and create derivative works of the software or any portion of the software, and you may copy and distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.
 *
 * NIST-developed software is expressly provided "AS IS." NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT, OR ARISING BY OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 *
 * You are solely responsible for determining the appropriateness of using and distributing the software and you assume all risks associated with its use, including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and the unavailability or interruption of operation. This software is not intended to be used in any situation where a failure could cause risk of injury or damage to property. The software developed by NIST employees is not subject to copyright protection within the United States.
*/

#ifndef PQCLEAN_FALCON512_CLEAN_API_H
#define PQCLEAN_FALCON512_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_FALCON512_CLEAN_CRYPTO_SECRETKEYBYTES   1281
#define PQCLEAN_FALCON512_CLEAN_CRYPTO_PUBLICKEYBYTES   897
#define PQCLEAN_FALCON512_CLEAN_CRYPTO_BYTES            752

#define PQCLEAN_FALCON512_CLEAN_CRYPTO_ALGNAME          "Falcon-512"

#define PQCLEAN_FALCONPADDED512_CLEAN_CRYPTO_BYTES      666 // used in signature verification

/*
 * Generate a new key pair. Public key goes into pk[], private key in sk[].
 * Key sizes are exact (in bytes):
 *   public (pk): PQCLEAN_FALCON512_CLEAN_CRYPTO_PUBLICKEYBYTES
 *   private (sk): PQCLEAN_FALCON512_CLEAN_CRYPTO_SECRETKEYBYTES
 *
 * Return value: 0 on success, -1 on error.
 */
int PQCLEAN_FALCON512_CLEAN_crypto_sign_keypair(
    uint8_t *pk, uint8_t *sk);

/*
 * Compute a signature on a provided message (m, mlen), with a given
 * private key (sk). Signature is written in sig[], with length written
 * into *siglen. Signature length is variable; maximum signature length
 * (in bytes) is PQCLEAN_FALCON512_CLEAN_CRYPTO_BYTES.
 *
 * sig[], m[] and sk[] may overlap each other arbitrarily.
 *
 * Return value: 0 on success, -1 on error.
 */
int PQCLEAN_FALCON512_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

/*
 * Verify a signature (sig, siglen) on a message (m, mlen) with a given
 * public key (pk).
 *
 * sig[], m[] and pk[] may overlap each other arbitrarily.
 *
 * Return value: 0 on success, -1 on error.
 */
int PQCLEAN_FALCON512_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

/*
 * Compute a signature on a message and pack the signature and message
 * into a single object, written into sm[]. The length of that output is
 * written in *smlen; that length may be larger than the message length
 * (mlen) by up to PQCLEAN_FALCON512_CLEAN_CRYPTO_BYTES.
 *
 * sm[] and m[] may overlap each other arbitrarily; however, sm[] shall
 * not overlap with sk[].
 *
 * Return value: 0 on success, -1 on error.
 */
int PQCLEAN_FALCON512_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

/*
 * Open a signed message object (sm, smlen) and verify the signature;
 * on success, the message itself is written into m[] and its length
 * into *mlen. The message is shorter than the signed message object,
 * but the size difference depends on the signature value; the difference
 * may range up to PQCLEAN_FALCON512_CLEAN_CRYPTO_BYTES.
 *
 * m[], sm[] and pk[] may overlap each other arbitrarily.
 *
 * Return value: 0 on success, -1 on error.
 */
int PQCLEAN_FALCON512_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk);

#endif

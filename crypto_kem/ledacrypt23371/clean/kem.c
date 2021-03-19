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

#include "api.h"
#include "niederreiter_keygen.h"
#include "niederreiter_encrypt.h"
#include "niederreiter_decrypt.h"
#include "rng.h"
#include "fips202.h"
#include <string.h>
/* Generates a keypair - pk is the public key and sk is the secret key. */
int PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_keypair(uint8_t *pk,
                                                   uint8_t *sk )
{

   key_gen_niederreiter((publicKeyNiederreiter_t *const) pk,
                        (privateKeyNiederreiter_t *const) sk);

   return 0; // NIST convention: 0 == zero errors
} // end PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_keypair

/*----------------------------------------------------------------------------*/
/* Encrypt - pk is the public key,
 *           ct is a key encapsulation message (ciphertext),
 *           ss is the shared secret.
 */

int PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_enc(uint8_t *ct,
                                               uint8_t *ss,
                                               const uint8_t *pk )
{

   encrypt_niederreiter_indcca2((unsigned char *const)
                                ct, /* ciphertext - output   */
                                (unsigned char *const) ss,  /* shared secret - output*/
                                (const publicKeyNiederreiter_t *const) pk);

   return 0; // NIST convention: 0 == zero errors
} //end PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_enc

/*----------------------------------------------------------------------------*/
/* Decrypt - ct is a key encapsulation message (ciphertext),
 *           sk is the private key,
 *           ss is the shared secret
 */

int PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_dec(uint8_t *ss,
                                               const uint8_t *ct,
                                               const uint8_t *sk)
{

   decrypt_niederreiter_indcca2((unsigned char *const) ss,
                                (const unsigned char *const) ct,
                                (const privateKeyNiederreiter_t *const) sk);

   return 0; // NIST convention: 0 == zero errors
} // end PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_dec

/*----------------------------------------------------------------------------*/

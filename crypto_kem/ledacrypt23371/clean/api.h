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

#ifndef PQCLEAN_LEDACRYPTXXXX_CLEAN_API_H
#define PQCLEAN_LEDACRYPTXXXX_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>


#define CRYPTO_ALGNAME "LEDA"

/* required bytes of input randomness */
#define  PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_RANDOMBYTES 24      //TRNG_BYTE_LENGTH

/* size in bytes of the secret key */
#define  PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_SECRETKEYBYTES 50     //(TRNG_BYTE_LENGTH*2)+2

/* size in bytes of the public key */
#define  PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_PUBLICKEYBYTES 2928    // DIGIT Mtr[(N0-1)*[(P+DIGIT_b-1)/DIGIT_b]]

/* size in bytes of the shared secret */
#define  PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_BYTES 32        //HASH_BYTE_LENGTH

/*size in bytes of the ciphertext*/
#define  PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_CIPHERTEXTBYTES 2952     // [(P+DIGIT_b-1)/DIGIT_b]]*DIGIT_SIZE_B+PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_RANDOMBYTES)


/* Generates a keypair - pk is the public key and sk is the secret key. */
int PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_keypair(uint8_t *pk,
                                                   uint8_t *sk );

/* Encrypt - pk is the public key, ct is a key encapsulation message
  (ciphertext), ss is the shared secret.*/
int PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_enc(uint8_t *ct,
                                               uint8_t *ss,
                                               const uint8_t *pk );


/* Decrypt - ct is a key encapsulation message (ciphertext), sk is the private
   key, ss is the shared secret */

int PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_dec(uint8_t *ss,
                                               const uint8_t *ct,
                                               const uint8_t *sk );

#endif
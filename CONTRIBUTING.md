# Contributing new schemes to PQClean

## Why contribute to PQClean

PQClean hopes to provide your scheme to people who want to integrate post-quantum cryptography into their own libraries and applications.
But our extensive testing framework might also help you catch bugs in your implementation, that might have otherwise gone unnoticed.
We run our builds on (emulated) ARMv7, ARMv8, x86 and amd64.
Also, we apply static and dynamic analysis tools.

## Adding your scheme

For this text, we will assume that you want to contribute a **kem** to PQClean.
For a signature scheme, these steps are equivalent, but the API is slightly different.
See the section [API](#API) below.

1. Fork our repository. You will be creating a pull request soon.
    * **Tip:** Do not wait until you think you have gotten everything perfect, before you open the pull request.
      We set up things so Github and the CI environment will give you feedback and guidance on the steps to follow.

2. Create the following folder structure: `crypto_kem/yourschemename/clean`.
We follow the SUPERCOP layout, so please create a separate folder for each parameter set.
For now, we only accept **pure, portable C code**

3. Create a ``META.yml`` file in ``crypto_kem/yourschemename/`` following this template:
```yaml
name: Name
type: <kem|signature>
claimed-nist-level: <N>
length-public-key: <N>
length-ciphertext: <N>
testvectors-sha256: sha256sum of output of testvectors
principal-submitter: Eve
auxiliary-submitters:
  - Alice
  - Bob
  - ...
implementations:
  - name: clean
    version: <some version indicator>
```
This file needs to be valid [YAML](https://yaml.org/).

4. Put your scheme into ``crypto_kem/yourschemename/clean``.
    1. Make sure all symbols are prefixed with ``PQCLEAN_YOURSCHEME_CLEAN_``
    2. Include ``api.h`` into your scheme with the symbols specified in the section [API](#API):

5. Create ``Makefile`` and ``Makefile.Microsoft_nmake`` files to compile your scheme as static library.
    * We suggest you take these from ``crypto_kem/kyber768/clean`` and modify them to suit your scheme.

6. Add a ``LICENSE`` file to your implementation folder.

7. Commit everything and push it to your fork

8. Open a pull request on our repository and process the feedback given to you by the CI environment.
   The pull request will also set up a checklist for you and us to follow.

## API

These items should be available in your ``api.h`` file.
Please make sure your ``api.h`` file does not include any other files.

### KEMs
* ``int PQCLEAN_YOURSCHEME_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);``
* ``int PQCLEAN_YOURSCHEME_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);``
* ``int PQCLEAN_YOURSCHEME_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);``
* ``define`` macros
  * ``CRYPTO_SECRETKEYBYTES``
  * ``CRYPTO_PUBLICKEYBYTES``
  * ``CRYPTO_CIPHERTEXTBYTES``
  * ``CRYPTO_BYTES``
  * ``CRYPTO_ALGNAME``

### Signature schemes
* ``int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);``
* ``int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *msg, unsigned long long len, const unsigned char *sk);``
* ``int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign_open(unsigned char *m, unsigned long long *mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk);``
* ``define`` macros
  * ``CRYPTO_SECRETKEYBYTES``
  * ``CRYPTO_PUBLICKEYBYTES``
  * ``CRYPTO_BYTES``
  * ``CRYPTO_ALGNAME``

#### Return codes
Your schemes should return 0 on success, or a negative value on failure.
Notably, ``crypto_sign_open`` should return ``-1`` if signature verification failed.

# Contributing to the framework of PQClean

We also welcome contributions to the testing framework.
Open an issue or pull request on Github and we will review your suggestion.
In general, we are always looking to improve the experience of submitters of schemes and of people consuming the implementations collected by this project.
Please do bear in mind the intentions of this project.

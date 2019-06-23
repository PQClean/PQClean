Contributing new schemes to PQClean
===================================

Why contribute to PQClean
-------------------------

PQClean hopes to provide your scheme to people who want to integrate post-quantum cryptography into their own libraries and applications. But our extensive testing framework might also help you catch bugs in your implementation, that might have otherwise gone unnoticed. We run our builds on (emulated) ARMv7, ARMv8, 32-bit PowerPC, x86 and amd64. Also, we apply static and dynamic analysis tools.

Adding your scheme
------------------

For this text, we will assume that you want to contribute a **key encapsulation mechanism (KEM)** to PQClean. For a signature scheme, these steps are equivalent, but the API is slightly different.
See the section [API](#API) below.

1. Fork our repository. You will be creating a pull request soon.
    * **Tip:** Do not wait until you think you have gotten everything perfect, before you open the pull request. We set up things so Github and the CI environment will give you feedback and guidance on the steps to follow.

2. Create the following folder structure: `crypto_kem/yourschemename/clean`. We follow the SUPERCOP layout, so please create a separate folder under `crypto_kem` for each parameter set.

    For now, we only accept **pure, portable C code**. Our coding conventions impose certain constraints on the C code -- C99 code, fixed sized integer types (e.g., `uint64_t` rather than `unsigned long long`), and more.  See README.md for more information.

3. Create a `META.yml` file in `crypto_(kem|sign)/yourschemename` following this template:

    ```yaml
    name: Name
    type: <kem|signature>
    claimed-nist-level: <N>
    claimed-security: IND-CPA/IND-CCA2  # KEM only
    length-public-key: <N>          # KEM and signature
    length-secret-key: <N>          # KEM and signature
    length-ciphertext: <N>          # KEM only
    length-shared-secret: <N>       # KEM only
    length-signature: <N>           # Signature only
    nistkat-sha256: sha256sum of 1st NIST KAT test case # KEM and signature
    testvectors-sha256: sha256sum of output of testvectors # Signature only
    principal-submitters:
      - Alice
      - Bob
      - ...
    auxiliary-submitters: # optional
      - Alice
      - Bob
      - ...
    implementations:
      - name: clean
        version: <some version indicator>
    ```

    This file needs to be valid [YAML](https://yaml.org/).

4. Put your scheme's C source code into `crypto_kem/yourschemename/clean`.

    1. Make sure all symbols are prefixed with `PQCLEAN_YOURSCHEME_CLEAN_`.
    2. Include `api.h` into your scheme with the symbols specified in the section [API](#API). Make sure it does not include other files.
    3. We use `astyle` to format code. You may consider running the following command on your submission:
    ```
    astyle --project crypto_kem/yourschemename/clean/*.[ch]
    ```
    4. You may run the tests in the `tests/` folder. See the `README` for how to run the test suite.

5. Create `Makefile` and `Makefile.Microsoft_nmake` files to compile your scheme as static library.
    * We suggest you copy these from `crypto_kem/kyber768/clean` and modify them to suit your scheme.

6. Add a `LICENSE` file to your implementation folder.

7. Commit everything and push it to your fork.

8. Open a pull request on our Github repository and process the feedback given to you by the CI environment.  The pull request will also set up a checklist for you and us to follow.  Feel free to ask us questions via the pull request.

API
---

These items should be available in your `api.h` file.

### KEMs

Functions:

```c
int PQCLEAN_YOURSCHEME_CLEAN_crypto_kem_keypair(
    uint8_t *pk, uint8_t *sk);
int PQCLEAN_YOURSCHEME_CLEAN_crypto_kem_enc(
    uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int PQCLEAN_YOURSCHEME_CLEAN_crypto_kem_dec(
    uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
```

`#define` macros:

* `CRYPTO_SECRETKEYBYTES`
* `CRYPTO_PUBLICKEYBYTES`
* `CRYPTO_CIPHERTEXTBYTES`
* `CRYPTO_BYTES`
* `CRYPTO_ALGNAME`

### Signature schemes

Functions:

```c
int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign_keypair(
    uint8_t *pk, uint8_t *sk);
int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *msg, size_t len,
    const uint8_t *sk);
int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen,
    const uint8_t *pk);
int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *sk);
int PQCLEAN_YOURSCHEME_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *pk);
```

`#define` macros:

* `PQCLEAN_YOURSCHEME_CLEAN_CRYPTO_SECRETKEYBYTES`
* `PQCLEAN_YOURSCHEME_CLEAN_CRYPTO_PUBLICKEYBYTES`
* `PQCLEAN_YOURSCHEME_CLEAN_CRYPTO_ALGNAME`
* `PQCLEAN_YOURSCHEME_CLEAN_CRYPTO_BYTES`

for KEMs, additionally define:

* `PQCLEAN_YOURSCHEME_CLEAN_CRYPTO_CIPHERTEXTBYTES`

Please make sure your `api.h` file does not include any other files.

### Return codes

Your schemes should return 0 on success, or a negative value on failure.
Notably, `crypto_sign_open` should return `-1` if signature verification failed.

Contributing to the framework of PQClean
========================================

We also welcome contributions to the testing framework. Open an issue or pull request on Github and we will review your suggestion. In general, we are always looking to improve the experience of submitters of schemes and of people consuming the implementations collected by this project.

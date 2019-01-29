# PQClean

[![Build Status](https://travis-ci.com/PQClean/PQClean.svg?branch=master)](https://travis-ci.com/PQClean/PQClean)

**PQClean**, in short, is an effort to collect **clean** implementations of the post-quantum
schemes that are in the 
[NIST post-quantum project](https://csrc.nist.gov/projects/post-quantum-cryptography).
The goal of PQClean is to provide *standalone implementations* that 
* can easily be integrated into libraries such as [liboqs](https://openquantumsafe.org/#liboqs) or [libpqcrypto](https://libpqcrypto.org/);
* can efficiently upstream into higher-level protocol integration efforts such as [Open Quantum Safe](https://openquantumsafe.org/#integrations);
* can easily be integrated into benchmarking frameworks such as [SUPERCOP](https://bench.cr.yp.to/supercop.shtml);
* can easily be integrated into frameworks targeting embedded platforms such as [pqm4](https://github.com/mupq/pqm4);
* are suitable starting points for architecture-specific optimized implementations;
* are suitable starting points for evaluation of implementation security; and
* are suitable targets for formal verification.

What PQClean is **not** aiming for is
* a build system producing an integrated library of all schemes;
* including benchmarking of implementations; and
* including integration into higher-level applications or protocols.

As a first main target, we are collecting C implementations that fulfill the requirements
listed below. 

## Requirements on C implementations that are automatically checked

_The checking of items on this list is still being developed. Checked items should be working._

* [x] Code is valid C99
* [x] Passes functional tests
* [x] API functions do not write outside provided buffers
* [x] Compiles with `-Wall -Wextra -Wpedantic -Werror` with `gcc` and `clang`
* [x] Consistent test vectors across runs
* [ ] Consistent test vectors on big-endian and little-endian machines
* [ ] Consistent test vectors on 32-bit and 64-bit machines
* [X] No errors/warnings reported by valgrind
* [X] No errors/warnings reported by address sanitizer
* [ ] Only dependencies:
  * [x] `fips202.c`
  * [x] `sha2.c`
  * [ ] `aes.c`
  * [x] `randombytes.c`
* [ ] API functions return `0` on success, negative on failure
    * [x] 0 on success
    * [ ] Negative on failure (within restrictions of FO transform).
* [ ] No dynamic memory allocations
* [ ] No branching on secret data (dynamically checked using valgrind)
* [ ] No access to secret memory locations (dynamically checked using valgrind)
* [ ] Separate subdirectories (without symlinks) for each parameter set of each scheme
* [ ] Builds under Linux, MacOS, and Windows
    * [x] Linux
    * [x] MacOS
    * [ ] Windows
* [ ] Makefile-based build for each separate scheme
* [ ] Makefile-based build for Windows (`nmake`)
* [x] All exported symbols are namespaced with `PQCLEAN_SCHEMENAME_`
* [ ] Each implementation comes with a `LICENSE` file (see below)
* [x] Each scheme comes with a `META.yml` file giving details about version of the algorithm, designers
    * [x] Each individual implementation is specified in `META.yml`.


## Requirements on C implementations that are manually checked

* Makefiles without explicit rules (rely on implicit, built-in rules)
* `#ifdef`s only for header encapsulation
* No stringification macros
* Output-parameter pointers in functions are on the left
* `const` arguments are labeled as `const`
* All exported symbols are namespaced in place
* All integer types are of fixed size, using `stdint.h` types (including `uint8_t` instead of `unsigned char`)
* Integers used for indexing are of size `size_t`
* variable declarations at the beginning (except in `for (size_t i=...`)


## Clean C implementations currently in PQClean

Currently, the continuous-integration and testing environment of PQClean is still work in progress 
and as a consequence PQClean does not yet have many implementations.

<!--
 Currently, PQClean includes clean C implementations of the following KEMs:

 * [Kyber-512](https://pq-crystals.org/kyber/)
 * [Kyber-768](https://pq-crystals.org/kyber/)
 * [Kyber-1024](https://pq-crystals.org/kyber/)

 Currently, PQClean includes clean C implementations of the following signature schemes:

 * [Dilithium-III](https://pq-crystals.org/dilithium/)
-->

## API used by PQClean

PQClean is essentially using the same API as required for the NIST reference implementations, 
which is also used by SUPERCOP and by libpqcrypto. The only two differences to that API are
the following:
* All lengths are passed as type `size_t` instead of `unsigned long long`; and
* Signatures offer two additional functions that follow the "traditional" approach used
in most software stacks of computing and verifying signatures instead of producing and
recovering signed messages. Specifically, those functions have the following name and signature:

```
int crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
```

## License

Each subdirectory containing implementations contains a LICENSE file stating under what license
that specific implementation is released. All other code for testing etc. in this repository
is released under the conditions of [CC0](http://creativecommons.org/publicdomain/zero/1.0/).

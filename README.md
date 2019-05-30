# PQClean

[![Build Status on Travis CI](https://travis-ci.com/PQClean/PQClean.svg?branch=master)](https://travis-ci.com/PQClean/PQClean)
[![Build Status on Appveyor](https://ci.appveyor.com/api/projects/status/186ky7yb9mlqj3io/branch/master?svg=true)](https://ci.appveyor.com/project/PQClean/pqclean/branch/master)
[![Build Status on CircleCI](https://circleci.com/gh/PQClean/PQClean/tree/master.svg?style=svg)](https://circleci.com/gh/PQClean/PQClean/tree/master)

**PQClean**, in short, is an effort to collect **clean** implementations of the post-quantum
schemes that are in the
[NIST post-quantum project](https://csrc.nist.gov/projects/post-quantum-cryptography).
The goal of PQClean is to provide *standalone implementations* that

* can easily be integrated into libraries such as [liboqs](https://openquantumsafe.org/#liboqs) or [libpqcrypto](https://libpqcrypto.org/);
* can efficiently upstream into higher-level protocol integration efforts such as [Open Quantum Safe](https://openquantumsafe.org/#integrations);
* can easily be integrated into benchmarking frameworks such as [SUPERCOP](https://bench.cr.yp.to/supercop.html);
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
Please also review our [guidelines for contributors](CONTRIBUTING.md) if you are interested in adding a scheme to PQClean.

## Requirements on C implementations that are automatically checked

_The checking of items on this list is still being developed. Checked items should be working._

* [x] Code is valid C99
* [x] Passes functional tests
* [x] API functions do not write outside provided buffers
* [x] `api.h` cannot include external files
* [x] Compiles with `-Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes` with `gcc` and `clang`
* [x] `#if`/`#ifdef`s only for header encapsulation
* [x] Consistent test vectors across runs
* [x] Consistent test vectors on big-endian and little-endian machines
* [x] Consistent test vectors on 32-bit and 64-bit machines
* [x] No errors/warnings reported by valgrind
* [x] No errors/warnings reported by address sanitizer
* [x] Only dependencies: `fips202.c`, `sha2.c`, `aes.c`, `randombytes.c`
* [x] API functions return `0` on success
* [x] No dynamic memory allocations (including variable-length arrays)
* [ ] No branching on secret data (dynamically checked using valgrind)
* [ ] No access to secret memory locations (dynamically checked using valgrind)
* [x] Separate subdirectories (without symlinks) for each parameter set of each scheme
* [x] Builds under Linux, MacOS, and Windows
    * [x] Linux
    * [x] MacOS
    * [x] Windows
* [x] Makefile-based build for each separate scheme
* [x] Makefile-based build for Windows (`nmake`)
* [x] All exported symbols are namespaced with `PQCLEAN_SCHEMENAME_`
* [x] Each implementation comes with a `LICENSE` file (see below)
* [x] Each scheme comes with a `META.yml` file giving details about version of the algorithm, designers
    * [x] Each individual implementation is specified in `META.yml`.


## Requirements on C implementations that are manually checked

* Minimalist Makefiles
* No stringification macros
* Output-parameter pointers in functions are on the left
* `const` arguments are labeled as `const`
* All exported symbols are namespaced in place
* Integer types are of fixed size where relevant, using `stdint.h` types
* Integers used for indexing memory are of size `size_t`
* Variable declarations at the beginning (except in `for (size_t i=...`)


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

## Building PQClean

As noted above, PQClean is **not** meant to be built as a single library: it is a collection of source code that can be easily integrated into other libraries.  The PQClean repository includes various test programs which do build various files, but you should not use the resulting binaries for any purpose.

List of required dependencies: ``gcc or clang, make, python3, python-yaml library, valgrind, astyle (>= 3.0)``.

## Using source code from PQClean in your own project

Each implementation directory in PQClean (e.g., crypto\_kem/kyber768\clean) can be extracted for use in your own project.  You will need to:

1. Copy the source code from the implementation's directory into your project.
2. Add the files to your project's build system.
3. Provide instantiations of any of the common cryptographic algorithms used by the implementation.  This likely includes `common/randombytes.h` (a cryptographic random number generator), and possibly `common/sha2.h` (the SHA-2 hash function family) and `common/fips202.h` (the SHA-3 hash function family).

Regarding #2, adding the files to your project's build system, each implementation in PQClean is accompanied by example two makefiles that show how one could build the files for that implementation:

- The file `Makefile` which can be used with GNU Make, BSD Make, and possibly others.
- The file `Makefile.Microsoft_nmake` which can be used with Visual Studio's nmake.

## Projects integrating PQClean-distributed source code

The following projects consume implementations from PQClean and provide their own wrappers around the implementations.
Their integration strategies may serve as examples for your own projects.

- **[pqcrypto crate](https://github.com/rustpq/pqcrypto)**: Rust integration that automatically generates wrappers from PQClean source code.
- **[mupq](https://github.com/mupq/)**: Runs the implementations from PQClean as reference implementations to compare with microcontroller-optimized code.
- **[Open Quantum Safe](https://github.com/open-quantum-safe/)**: The Open Quantum Safe project integrates implementations from PQClean into their [liboqs](https://github.com/open-quantum-safe/liboqs) C library, which then exposes them via [C++](https://github.com/open-quantum-safe/liboqs-cpp), [C# / .NET](https://github.com/open-quantum-safe/liboqs-dotnet), and [Python](https://github.com/open-quantum-safe/liboqs-python) wrappers, as well as to forks of [OpenSSL](https://github.com/open-quantum-safe/openssl) and [OpenSSH](https://github.com/open-quantum-safe/openssh-portable).

## License

Each subdirectory containing implementations contains a `LICENSE` file stating under what license that specific implementation is released.
The files in `common` contain licensing information at the top of the file (and are currently either public domain or MIT).
All other code in this repository is released under the conditions of [CC0](http://creativecommons.org/publicdomain/zero/1.0/).

## Running tests locally

See https://github.com/PQClean/PQClean/wiki/Test-framework for details about the PQClean test framework.

While we run extensive automatic testing on [Circle CI][circleci-pqc] (Linux builds), [Travis CI][travis-pqc] (OS X builds) and [Appveyor][appveyor-pqc] (Windows builds), most tests can also be run locally.
To do this, make sure the following is installed:

* Python 3.5+
* `nosetests` or `nose2` (either for Python 3)

You will also need to make sure the submodules are initialized by running:

```
git submodule update --init
```

Run the Python-based tests by going into the `test` directory and running `nosetests -v` or `nose2 -B -v`, depending on what you installed.
If you have the `rednose` plugin for `nosetests` installed, run `nosetests --rednose` to get colored output.

You may also run `python3 <testmodule>` where `<testmodule>` is any of the files starting with `test_` in the `test/` folder.

[circleci-pqc]: https://circleci.com/gh/PQClean/PQClean/
[travis-pqc]: https://travis-ci.com/PQClean/PQClean/
[appveyor-pqc]: https://ci.appveyor.com/project/PQClean/pqclean

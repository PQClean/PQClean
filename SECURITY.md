# A note on the security of the included implementations and schemes

This project contains (reference) implementations of cryptographic libraries.
We do not make any security claims about the code included in PQClean.
In the current state, we distribute reference implementations with minor modifications.
We did not perform any extensive security analyses.
This code is suitable for experimental or scientific use.
We recommend careful expert code review before using any of the included implementations in production environments.

See also the [NIST PQC Forum][forum] for discussion about the cryptographic schemes included in PQClean.

## Current and past security issues

We will attempt to document security vulnerabilities made known to us on a best-effort basis.
If an issue is marked with a date, the issue has been resolved since any commits made after that date.

Again, we emphasise that the code in this repository has not seen any formal analysis or audit!
Use at your own risk.

### Historic issues
* LEDAcryptKEM `leaktime` implementations are known to not be constant-time and expected to have timing side channel vulnerabilities.
  * LEDA support has since been dropped from PQClean as it got eliminated from the NIST PQC standardization project in round 3.
* Rainbow level I has been shown to be insecure by Ward Beullens.
  * Rainbow was eliminated from the NIST PQC standardization project and removed from PQClean in round 4.

<!-- new date line
### 2019-XX-XX
-->

### 2024-01-25
* Kyber used division operations that might leak side-channel information.
  [PR #534](https://github.com/PQClean/PQClean/pull/534) addressed this for the `clean` and `avx2` implementations.
  Note that the `aarch64` implementation is waiting for [PR#527](https://github.com/PQClean/PQClean/pull/527) to get fixed.

### 2020-12-11
* The fix of the timing leak in the CCA transform of FrodoKEM in [PR #303](https://github.com/PQClean/PQClean/pull/303) was ineffective. The FrodoKEM team released another [fix](https://github.com/microsoft/PQCrypto-LWEKE/commit/669522db63850fa64d1a24a47e138e80a59349db) which was ported to PQClean in [PR #367](https://github.com/PQClean/PQClean/pull/367).

### 2020-06-19
* A potential timing leak was present in the FrodoKEM decapsulation routine, as identified by [Guo, Johansson, and Nilsson](https://eprint.iacr.org/2020/743).  This was fixed in [PR #303](https://github.com/PQClean/PQClean/pull/303).

### 2019-09-24
* All Falcon implementations before [PR #235][PR 235] got merged were insecure. See [EPRINT report 2019/893][2019/893].

### 2019-09-10
* The included incremental `sha512` implementation was calling `crypto_hashblocks_sha256` before 2019-9-10.
  This lead to an insufficient security level of the results of this hash function.
  The function was not used in any implementations, though.
  See [PR #232][PR 232].


[2019/893]: https://eprint.iacr.org/2019/893
[forum]: https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Email-List

[PR 232]: https://github.com/PQClean/PQClean/pull/232
[PR 235]: https://github.com/PQClean/PQClean/pull/235

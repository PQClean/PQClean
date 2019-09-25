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

### Open issues
* LEDAcryptKEM `leaktime` implementations are known to not be constant-time and expected to have timing side channel vulnerabilities.

<!-- new date line
### 2019-XX-XX
-->

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

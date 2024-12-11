# PQClean Security Notice

## Project Scope

PQClean offers reference and contributed implementations of cryptographic algorithms, primarily for academic and experimental purposes. 
It is important to understand that these implementations have not been subjected to rigorous security audits or formal security validations. 
PQClean does not modify or alter the algorithms provided by their upstream sources. 
See the [NIST PQC Forum](https://csrc.nist.gov/Projects/Post-Quantum-Cryptography/Email-List) for discussion about the cryptographic schemes included in PQClean. 

## Security Considerations

Users should be aware that while PQClean aims to provide accurate and secure implementations, the project does not make explicit security claims. 
Currently, the code is best suited for research purposes and users should assess its suitability for their specific needs, especially in 
security-sensitive environments. Any use in a production environment should be preceded by a detailed expert security review. 

## Vulnerability Reporting

We endeavor to document known security vulnerabilities as they are brought to our attention. 
However, due to limited resources available to us, our capacity for rapid and comprehensive vulnerability management is limited and delivered at best effort. 
To report a vulnerability for a PQ algorithm, please open a [new issue](https://github.com/PQClean/PQClean/issues/new) and clearly document the specifics of the vulnerability. 
Resolved issues will be marked with the date of their resolution and will contain links to commits or merged PR-s which applied the relevant fixes. 
Reported vulnerabilities in the **Historic Issues** section have become irrelevant for PQClean due to the algorithms being removed from the library. 


<!-- new date line
### 2019-XX-XX
-->


## Resolved Issues

### 2024-12-11
* HQC decapsulation mishandled the secret key and, when given a malformed ciphertext, returned an incorrect shared secret.
  [PR #578](https://github.com/PQClean/PQClean/pull/578) addressed this.

### 2024-06-11
* Kyber used a conditional move that resulted in branch instructions leaking side-channel information for certain compilers.
  [PR #558](https://github.com/PQClean/PQClean/pull/558) addressed this for the `clean` and `avx2` implementations.

### 2024-01-25
* Kyber used division operations that might leak side-channel information.  
  [PR #534](https://github.com/PQClean/PQClean/pull/534) addressed this for the `clean` and `avx2` implementations.  
  Note that the `aarch64` implementation is waiting for [PR#527](https://github.com/PQClean/PQClean/pull/527) to get fixed.  

### 2019-09-24
* All Falcon implementations before [PR #235](https://github.com/PQClean/PQClean/pull/235) got merged were insecure.
  See [EPRINT report 2019/893](https://eprint.iacr.org/2019/893).

### 2019-09-10
* The included incremental `sha512` implementation was calling `crypto_hashblocks_sha256` before 2019-9-10.  
  This lead to an insufficient security level of the results of this hash function.  
  The function was not used in any implementations, though. See [PR #232](https://github.com/PQClean/PQClean/pull/232).


## Historic Issues

* The fix of the timing leak in the CCA transform of FrodoKEM in [PR #303](https://github.com/PQClean/PQClean/pull/303) was ineffective.  
  The FrodoKEM team released another [fix](https://github.com/microsoft/PQCrypto-LWEKE/commit/669522db63850fa64d1a24a47e138e80a59349db)
  which was ported to PQClean in [PR #367](https://github.com/PQClean/PQClean/pull/367).  

* A potential timing leak was present in the FrodoKEM decapsulation routine,  
  as identified by [Guo, Johansson, and Nilsson](https://eprint.iacr.org/2020/743). This was fixed in [PR #303](https://github.com/PQClean/PQClean/pull/303).  

* LEDAcryptKEM `leaktime` implementations are known to not be constant-time and expected to have timing side channel vulnerabilities.  
  LEDA support has since been dropped from PQClean as it got eliminated from the NIST PQC standardization project in round 3.  

* Rainbow level I has been shown to be insecure by Ward Beullens.  
  Rainbow was eliminated from the NIST PQC standardization project and removed from PQClean in round 4.  

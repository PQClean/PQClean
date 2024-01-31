# PQCleanAutomotive
PQCleanAutomotive is planed as a collection of post-quantum cryptographic algorithms for automotive microcontrollers. The post-quantum cryptographic algorithms are based on the [PQClean](https://github.com/PQClean/PQClean) repository, which is a clean collection of the [NIST post-quantum project](https://csrc.nist.gov/projects/post-quantum-cryptography).

## Security Note
There was no security risk analysis like TARA or similar carried out for QuantumSAR. The post-quantum cryptographic algorithms itself were evaluated by NIST.
The required random numbers are calculated using a software solution, which is only intended to serve as an example.

## Content
The base of PQCleanAutomotive is the PQClean repository with the latest commit [#504](https://github.com/PQClean/PQClean/commit/fb003a2a625c49f3090eec546b2383dcfa2c75d8). It includes the following post-quantum cryptographic algorithms:

* CRYSTALS-KYBER
* CRYSTALS-DILITHIUM
* SPHINCS+

The folloing post-quantum cryptographic algorithms are in work:

* FALCON

## Implementation
For the integration of PQCleanAutomotive, the file Crypto.c in the IAV quantumSAR repository contains an example of the use of the CRYSTALS-KYBER, CRYSTALS-DILITHIUM and SPHINCS+ algorithm.

The library requires an H-file containing all data types of the respective hardware platform (Platform_Types.h). The implementation from Infineon for the AURIX microcontroller family can be used as an example here: https://github.com/Infineon/AURIX_code_examples/blob/master/code_examples/Blinky_LED_1_KIT_TC397_TFT/Libraries/iLLD/TC39B/Tricore/Cpu/Std/Platform_Types.h

In the SPHINCS+ algorithm, dynamic arrays had to be replaced by static arrays in various functions for the microcontroller implementation.

## License
PQCleanAutomotive includes some third party libraries or modules that are licensed differently. In particular:

* CRYSTALS-KYBER: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* CRYSTALS-DILITHIUM: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* SPHINCS+: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* Fips202: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* CommonLib: Apache License, Version 2.0

Copyright [2024] [IAV GmbH]

Licensed under the Apache License, Version 2.0 (the "License");\
you may not use this file except in compliance with the License.\
You may obtain a copy of the License at\
\
&nbsp;&nbsp;&nbsp;[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)\
\
Unless required by applicable law or agreed to in writing, software\
distributed under the License is distributed on an "AS IS" BASIS,\
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\
See the License for the specific language governing permissions and\
limitations under the License.

## MISRA
PQCleanAutomotive and the post-quantum cryptographic algorithms are checked with the 2012 version of [MISRA](https://misra.org.uk/app/uploads/2021/06/MISRA-C-2012-Permits-First-Edition.pdf).

## Patent
The Encryption PQC algorithm CRYSTALS-KYBER is one of the candidate algorithms for public-key post-quantum standards. There are two patent portfolios listed in a patent license agreement published by NIST.  
https://csrc.nist.gov/csrc/media/Projects/post-quantum-cryptography/documents/selected-algos-2022/nist-pqc-license-summary-and-excerpts.pdf
Please check the license condition before any commercial implementation. IAV isn’t owner, applicant, licensor or sub-licensor of the patent portfolios. It is any implementers sole responsibility to do the final license check for all implementations of CRYSTALS-KYBER. IAV is not responsible for possible infringements of third party rights.

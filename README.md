# PQCleanAutomotive
PQCleanAutomotive is planned as a collection of post-quantum cryptographic algorithms for automotive microcontrollers. The post-quantum cryptographic algorithms are based on the [PQClean](https://github.com/PQClean/PQClean) repository, which is a clean collection of the [NIST post-quantum project](https://csrc.nist.gov/projects/post-quantum-cryptography).

## Security Note
There was no security risk analysis like TARA or similar carried out for QuantumSAR. The post-quantum cryptographic algorithms itself were evaluated by NIST.
The required random numbers are calculated using a software solution, which is only intended to serve as an example.

## Content
The base of PQCleanAutomotive is the PQClean repository with the latest commit [#504](https://github.com/PQClean/PQClean/commit/fb003a2a625c49f3090eec546b2383dcfa2c75d8). It includes the following post-quantum cryptographic algorithms:

* CRYSTALS-KYBER
* CRYSTALS-DILITHIUM
* SPHINCS+
* FALCON

## Implementation
For the integration of PQCleanAutomotive, the file Crypto.c in the IAV quantumSAR repository contains an example of the use of the CRYSTALS-KYBER, CRYSTALS-DILITHIUM, SPHINCS+ and FALCON algorithm.

In the SPHINCS+ algorithm, dynamic arrays had to be replaced by static arrays in various functions for the microcontroller implementation.

## License
PQCleanAutomotive includes some third party libraries or modules that are licensed differently. In particular:

* CRYSTALS-KYBER: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* CRYSTALS-DILITHIUM: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* SPHINCS+: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
* FALCON: MIT License
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
PQCleanAutomotive and the post-quantum cryptographic algorithms are checked with the 2012 version of [MISRA](https://misra.org.uk/app/uploads/2021/06/MISRA-C-2012-Permits-First-Edition.pdf). Not all MISRA findings could be resolved. An explanation has been written in the relevant places.
Note: Rule 1.1 of MISRA was globally disabled because the IAV headers in each file are detected as macros, leading to numerous false positives. Disabling this rule ensures that these headers are not incorrectly flagged as violations.

## Compiler
In this repository the post-quantum cryptographic algorithms are warning free compiled with TASKING Compiler with standard flags. 

<!-- This template will help you get your code into PQClean. -->

<!-- Type some lines about your submission -->


## Manually checked properties
<!-- These checkboxes serve for the maintainers of PQClean to verify your submission. Please do not check them yourself. -->

* [ ] `#ifdef`s only for header encapsulation
* [ ] `api.h` does not include other files
* [ ] No stringification macros
* [ ] Output-parameter pointers in functions are on the left
* [ ] Negative return values on failure of API functions (within restrictions of FO transform).
* [ ] `const` arguments are labeled as `const`
* [ ] (optional) All integer types are of fixed size, using `stdint.h` types (including `uint8_t` instead of `unsigned char`)
* [ ] Integers used for indexing are of size `size_t`
* [ ] variable declarations at the beginning (except in `for (size_t i=...`)

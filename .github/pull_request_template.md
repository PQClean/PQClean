<!-- This template will help you get your code into PQClean. -->

<!-- Type some lines about your submission -->

<!-- !! If you are not submitting a new scheme, we suggest removing the following lines!!  -->

#### Manually checked properties
* [ ] Generated Github workflow (run ``.github/workflows/generate_workflows.py``) (new schemes)
* [ ] No stringification macros
* [ ] Output-parameter pointers in functions are on the left
* [ ] Negative return values on failure of API functions (within restrictions of FO transform).
* [ ] variable declarations at the beginning (except in `for (size_t i=...`)
* Optional:
  * [ ] All integer types are of fixed size, using `stdint.h` types (including `uint8_t` instead of `unsigned char`)
  * [ ] Integers used for indexing are of size `size_t`

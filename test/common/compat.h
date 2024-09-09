/** Empty on purpose so preprocessor / compiler-specific stuff does not choke test_char.py **/

#define PQCLEAN_VLA(__t,__x,__s) __t __x[__s]
#define PQCLEAN_PREVENT_BRANCH_HACK(b)

#include "stddef.h"
#include <valgrind/memcheck.h>

void PQCLEAN_TIMECOP_DECLASSIFY(void *addr, size_t len);

void PQCLEAN_TIMECOP_DECLASSIFY(void *addr, size_t len) {
    VALGRIND_MAKE_MEM_DEFINED(addr, len);
}

#ifndef SPX_F1600X2_H
#define SPX_F1600X2_H

#include "context.h"
#include "params.h"
#include <stdint.h>
#define f1600_RC SPX_NAMESPACE(f1600_RC)
extern uint64_t f1600_RC[24];
extern void _f1600x2(uint64_t *a, uint64_t *rc);

#define f1600x2(s) do {_f1600x2((s), f1600_RC);} while(0)

#endif

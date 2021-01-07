#ifndef CONSTS_H
#define CONSTS_H

#include "alignment.h"
#include "params.h"

#define _8XQINV      0
#define _8XQ         8
#define _MASK       16
#define _8X23ONES   24
#define _8XDIV      32
#define _ZETAS      40
#define _ZETAS_INV 296

#define qdata DILITHIUM_NAMESPACE(_qdata)
typedef ALIGNED_INT32(568) aligned_int32x568_t;
extern const aligned_int32x568_t qdata;

#endif

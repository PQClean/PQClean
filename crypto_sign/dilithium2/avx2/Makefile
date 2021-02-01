# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium2_avx2.a
HEADERS=align.h api.h cdecl.h consts.h fips202x4.h ntt.h packing.h params.h poly.h polyvec.h rejsample.h rounding.h sign.h symmetric.h  shuffle.inc 
OBJECTS=consts.o fips202x4.o packing.o poly.o polyvec.o rejsample.o rounding.o sign.o symmetric-shake.o  f1600x4.o invntt.o ntt.o pointwise.o shuffle.o 
KECCAK4XDIR=../../../common/keccak4x
KECCAK4XOBJ=KeccakP-1600-times4-SIMD256.o
KECCAK4X=$(KECCAK4XDIR)/$(KECCAK4XOBJ)

CFLAGS=-mavx2 -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror \
          -Wmissing-prototypes -Wredundant-decls \
          -Wpointer-arith -Wshadow \
          -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) $(KECCAK4X)
	$(AR) -r $@ $(OBJECTS) $(KECCAK4X)

$(KECCAK4X):
	$(MAKE) -C $(KECCAK4XDIR) $(KECCAK4XOBJ)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

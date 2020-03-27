# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium2_avx2.a

SOURCES = fips202x4.c invntt.S nttconsts.c ntt.S packing.c pointwise.S poly.c \
		  polyvec.c reduce.S rejsample.c rounding.c sign.c stream.c
OBJECTS = fips202x4.o invntt.o nttconsts.o ntt.o packing.o pointwise.o poly.o \
		  polyvec.o reduce.o rejsample.o rounding.o sign.o stream.o
HEADERS = alignment.h api.h params.h sign.h polyvec.h poly.h packing.h ntt.h  \
		  nttconsts.h reduce.h rounding.h rejsample.h symmetric.h stream.h    \
		  fips202x4.h shuffle.inc cdecl.inc

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror \
	   -Wmissing-prototypes -Wredundant-decls -std=c99 \
	   -Wcast-align -Werror=shadow\
	   -mavx2 -mbmi -mpopcnt -I../../../common $(EXTRAFLAGS)

all: $(LIB)

KECCAK4XDIR=../../../common/keccak4x
KECCAK4XOBJ=KeccakP-1600-times4-SIMD256.o
KECCAK4X=$(KECCAK4XDIR)/$(KECCAK4XOBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) -c -o $@ $<

$(LIB): $(OBJECTS) $(KECCAK4X)
	$(AR) -r $@ $^

$(KECCAK4X):
	$(MAKE) -C $(KECCAK4XDIR) $(KECCAK4XOBJ)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
	$(MAKE) -C $(KECCAK4XDIR) clean


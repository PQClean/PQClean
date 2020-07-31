# This Makefile can be used with GNU Make or BSD Make

LIB=libkyber512_avx2.a
HEADERS=	 	\
	align.h     \
	api.h       \
	cbd.h       \
	cdecl.inc   \
	consts.h    \
	fips202x4.h \
	fq.inc      \
	indcpa.h    \
	kem.h       \
	ntt.h       \
	params.h    \
	poly.h      \
	polyvec.h   \
	reduce.h    \
	rejsample.h \
	shuffle.inc \
	symmetric.h \
	verify.h
OBJECTS=				\
	basemul.o           \
	cbd.o               \
	consts.o            \
	fips202x4.o         \
	fq.o                \
	indcpa.o            \
	invntt.o            \
	kem.o               \
	ntt.o               \
	poly.o              \
	polyvec.o           \
	rejsample.o         \
	shuffle.o           \
	symmetric-shake.o   \
	verify.o

KECCAK4XDIR=../../../common/keccak4x
KECCAK4XOBJ=KeccakP-1600-times4-SIMD256.o
KECCAK4X=$(KECCAK4XDIR)/$(KECCAK4XOBJ)

CFLAGS=-mavx2 -mbmi2 -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror \
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

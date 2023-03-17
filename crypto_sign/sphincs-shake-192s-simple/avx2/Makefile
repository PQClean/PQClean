# This Makefile can be used with GNU Make or BSD Make

LIB = libsphincs-shake-192s-simple_avx2.a

HEADERS = address.h api.h context.h fips202x4.h fors.h hash.h hashx4.h merkle.h nistapi.h params.h shake_offsets.h thash.h thashx4.h utils.h utilsx4.h wots.h wotsx4.h
OBJECTS = address.o context_shake.o fips202x4.o fors.o hash_shake.o hash_shakex4.o merkle.o sign.o thash_shake_simple.o thash_shake_simplex4.o utils.o utilsx4.o wots.o

CFLAGS  = -std=c99 -O3 -Wall -Wconversion -Werror -Wextra -Wmissing-prototypes -Wpedantic -Wredundant-decls -mavx2 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


KECCAK4XDIR=../../../common/keccak4x
KECCAK4XOBJ=KeccakP-1600-times4-SIMD256.o
KECCAK4X=$(KECCAK4XDIR)/$(KECCAK4XOBJ)

$(KECCAK4X):
	$(MAKE) -C $(KECCAK4XDIR) $(KECCAK4XOBJ)

$(LIB): $(OBJECTS) $(KECCAK4X)
	$(AR) -r $@ $(OBJECTS) $(KECCAK4X)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
	$(MAKE) -C $(KECCAK4XDIR) clean


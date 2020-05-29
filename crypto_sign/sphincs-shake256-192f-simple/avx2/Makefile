# This Makefile can be used with GNU Make or BSD Make

LIB=libsphincs-shake256-192f-simple_avx2.a

HEADERS = params.h address.h wots.h utils.h utilsx4.h fips202x4.h fors.h api.h hash.h hashx4.h hash_state.h thash.h thashx4.h 
OBJECTS =          address.o wots.o utils.o utilsx4.o fips202x4.o fors.o sign.o hash_shake256.o thash_shake256_simple.o hash_shake256x4.o thash_shake256_simplex4.o

KECCAK4XDIR=../../../common/keccak4x
KECCAK4XOBJ=KeccakP-1600-times4-SIMD256.o
KECCAK4X=$(KECCAK4XDIR)/$(KECCAK4XOBJ)

CFLAGS=-mavx2 -O3 -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) $(KECCAK4X)
	$(AR) -r $@ $(OBJECTS) $(KECCAK4X)

$(KECCAK4X):
	$(MAKE) -C $(KECCAK4XDIR) $(KECCAK4XOBJ)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
	$(MAKE) -C $(KECCAK4XDIR) clean

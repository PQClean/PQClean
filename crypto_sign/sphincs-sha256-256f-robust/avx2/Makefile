# This Makefile can be used with GNU Make or BSD Make

LIB=libsphincs-sha256-256f-robust_avx2.a

HEADERS = params.h address.h wots.h utils.h utilsx8.h sha256avx.h sha256x8.h fors.h api.h hash.h hashx8.h hash_state.h thash.h thashx8.h  sha256.h
OBJECTS =          address.o wots.o utils.o utilsx8.o sha256avx.o sha256x8.o fors.o sign.o hash_sha256.o thash_sha256_robust.o hash_sha256x8.o thash_sha256_robustx8.o sha256.o

CFLAGS=-mavx2 -O3 -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

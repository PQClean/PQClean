# This Makefile can be used with GNU Make or BSD Make

LIB=libsphincs-shake256-128s-robust_clean.a

HEADERS = params.h address.h wots.h utils.h fors.h api.h hash.h hash_state.h thash.h
OBJECTS =          address.o wots.o utils.o fors.o sign.o hash_shake256.o thash_shake256_robust.o

CFLAGS=-O3 -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

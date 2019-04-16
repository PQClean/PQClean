# This Makefile can be used with GNU Make or BSD Make

LIB=libsphincs-sha256-192f-robust_clean.a

HEADERS = params.h address.h wots.h utils.h fors.h api.h  hash.h thash.h sha256.h
OBJECTS =          address.o wots.o utils.o fors.o sign.o hash_sha256.o thash_sha256_robust.o sha256.o

CFLAGS=-Wall -Wconversion -Wextra -Wpedantic -Werror -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

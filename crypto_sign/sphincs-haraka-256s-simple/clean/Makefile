# This Makefile can be used with GNU Make or BSD Make

LIB = libsphincs-haraka-256s-simple_clean.a

HEADERS = address.h api.h context.h fors.h haraka.h haraka_offsets.h hash.h merkle.h nistapi.h params.h thash.h utils.h utilsx1.h wots.h wotsx1.h
OBJECTS = address.o context_haraka.o fors.o haraka.o hash_haraka.o merkle.o sign.o thash_haraka_simple.o utils.o utilsx1.o wots.o wotsx1.o

CFLAGS  = -std=c99 -O3 -Wall -Wconversion -Werror -Wextra -Wmissing-prototypes -Wpedantic -Wredundant-decls -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) 
	$(AR) -r $@ $(OBJECTS) 

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)


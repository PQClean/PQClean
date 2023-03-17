# This Makefile can be used with GNU Make or BSD Make

LIB = libsphincs-haraka-256f-simple_aesni.a

HEADERS = address.h api.h context.h fors.h haraka.h haraka_offsets.h harakax4.h hash.h hashx4.h merkle.h nistapi.h params.h thash.h thashx4.h utils.h utilsx4.h wots.h wotsx4.h
OBJECTS = address.o context_haraka.o fors.o haraka.o hash_haraka.o hash_harakax4.o merkle.o sign.o thash_haraka_simple.o thash_haraka_simplex4.o utils.o utilsx4.o wots.o

CFLAGS  = -std=c99 -O3 -Wall -Wconversion -Werror -Wextra -Wmissing-prototypes -Wpedantic -Wredundant-decls -maes -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) 
	$(AR) -r $@ $(OBJECTS) 

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)


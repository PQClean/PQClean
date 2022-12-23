# This Makefile can be used with GNU Make or BSD Make

LIB = libsphincs-sha2-128s-simple_avx2.a

HEADERS = address.h api.h context.h fors.h hash.h hashx8.h merkle.h nistapi.h params.h sha256avx.h sha256x8.h sha2_offsets.h thash.h thashx8.h utils.h utilsx8.h wots.h wotsx8.h
OBJECTS = address.o context_sha2.o fors.o hash_sha2.o hash_sha2x8.o merkle.o sha256avx.o sha256x8.o sign.o thash_sha2_simple.o thash_sha2_simplex8.o utils.o utilsx8.o wots.o

CFLAGS  = -std=c99 -O3 -Wall -Wconversion -Werror -Wextra -Wmissing-prototypes -Wpedantic -Wredundant-decls -mavx2 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) 
	$(AR) -r $@ $(OBJECTS) 

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)


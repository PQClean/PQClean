# This Makefile can be used with GNU Make or BSD Make

LIB = libsphincs-sha2-128s-simple_clean.a

HEADERS = address.h api.h context.h fors.h hash.h merkle.h nistapi.h params.h sha2_offsets.h thash.h utils.h utilsx1.h wots.h wotsx1.h
OBJECTS = address.o context_sha2.o fors.o hash_sha2.o merkle.o sign.o thash_sha2_simple.o utils.o utilsx1.o wots.o wotsx1.o

CFLAGS  = -std=c99 -O3 -Wall -Wconversion -Werror -Wextra -Wmissing-prototypes -Wpedantic -Wredundant-decls -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) 
	$(AR) -r $@ $(OBJECTS) 

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)


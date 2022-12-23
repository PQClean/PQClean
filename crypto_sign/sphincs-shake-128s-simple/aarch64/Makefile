# This Makefile can be used with GNU Make or BSD Make

LIB = libsphincs-shake-128s-simple_aarch64.a

HEADERS = address.h api.h context.h f1600x2.h fips202x2.h fors.h hash.h hashx2.h merkle.h nistapi.h params.h shake_offsets.h thash.h thashx2.h utils.h utilsx2.h wots.h wotsx2.h
OBJECTS = address.o context_shake.o f1600x2.o f1600x2_const.o fips202x2.o fors.o hash_shake.o hash_shakex2.o merkle.o sign.o thash_shake_simplex2.o utils.o utilsx2.o wots.o

CFLAGS  = -std=c99 -O3 -Wall -Wconversion -Werror -Wextra -Wmissing-prototypes -Wpedantic -Wredundant-decls -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


%.o: %.s
	$(AS) -o $@ $<

$(LIB): $(OBJECTS) 
	$(AR) -r $@ $(OBJECTS) 

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)


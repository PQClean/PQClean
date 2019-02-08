# This Makefile has been tested with GNU Make and BSD Make

LIB=libkyber768_clean.a
OBJECTS=cbd.o indcpa.o kem.o kex.o ntt.o poly.o polyvec.o precomp.o reduce.o verify.o

CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

$(LIB): $(OBJECTS)
    $(AR) -r $@ $(OBJECTS)

clean:
    $(RM) $(OBJECTS)
    $(RM) $(LIB)

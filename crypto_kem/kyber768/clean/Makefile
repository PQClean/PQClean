# This Makefile can be used with GNU Make or BSD Make

LIB=libkyber768_clean.a
OBJECTS=cbd.o indcpa.o kem.o ntt.o poly.o polyvec.o precomp.o reduce.o verify.o

CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium-iii_clean.a
OBJECTS=ntt.o packing.o poly.o polyvec.o reduce.o rounding.o sign.o

CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

$(LIB): $(OBJECTS)
    $(AR) -r $@ $(OBJECTS)

clean:
    $(RM) $(OBJECTS)
    $(RM) $(LIB)

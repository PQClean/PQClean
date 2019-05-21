# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium2_clean.a

SOURCES = sign.c polyvec.c poly.c packing.c ntt.c reduce.c rounding.c symmetric.c 
OBJECTS = sign.o polyvec.o poly.o packing.o ntt.o reduce.o rounding.o symmetric.o
HEADERS = api.h params.h sign.h polyvec.h poly.h packing.h ntt.h \
  reduce.h rounding.h symmetric.h 

CFLAGS=-O3 -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

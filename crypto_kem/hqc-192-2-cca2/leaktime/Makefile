# This Makefile can be used with GNU Make or BSD Make

LIB=libhqc-192-2-cca2_leaktime.a
HEADERS=api.h bch.h fft.h gf.h gf2x.h hqc.h parameters.h parsing.h repetition.h tensor.h vector.h
OBJECTS=bch.o fft.o gf.o gf2x.o hqc.o kem.o parsing.o repetition.o tensor.o vector.o

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

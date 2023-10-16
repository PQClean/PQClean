# This Makefile can be used with GNU Make or BSD Make

LIB=libhqc-128_clean.a
HEADERS=api.h code.h domains.h fft.h gf.h gf2x.h hqc.h parameters.h parsing.h reed_muller.h reed_solomon.h shake_ds.h shake_prng.h vector.h 
OBJECTS=code.o fft.o gf.o gf2x.o hqc.o kem.o parsing.o reed_muller.o reed_solomon.o shake_ds.o shake_prng.o vector.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wshadow -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

# This Makefile can be used with GNU Make or BSD Make

LIB=libhqc-rmrs-128_avx2.a
HEADERS=api.h code.h fft.h gf2x.h gf.h hqc.h parameters.h parsing.h reed_muller.h reed_solomon.h vector.h 
OBJECTS=code.o fft.o gf2x.o gf.o hqc.o kem.o parsing.o reed_muller.o reed_solomon.o vector.o 

CFLAGS=-O3 -mavx2 -mbmi -mpclmul -Wall -Wextra -Wshadow -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.s $(HEADERS)
	$(AS) -o $@ $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

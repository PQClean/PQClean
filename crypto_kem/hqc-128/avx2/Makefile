# This Makefile can be used with GNU Make or BSD Make

LIB=libhqc-128_avx2.a
HEADERS=alpha_table.h api.h bch.h code.h fft.h gen_matrix.h gf2x.h gf.h hqc.h parameters.h parsing.h repetition.h vector.h 
OBJECTS=bch.o code.o fft.o gf2x.o gf.o hqc.o kem.o parsing.o repetition.o vector.o 

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

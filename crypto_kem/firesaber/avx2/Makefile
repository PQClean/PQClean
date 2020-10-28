# This Makefile can be used with GNU Make or BSD Make

LIB=libfiresaber_avx2.a
HEADERS=api.h cbd.h pack_unpack.h poly.h SABER_indcpa.h SABER_params.h verify.h 
OBJECTS=cbd.o kem.o pack_unpack.o poly.o poly_mul.o SABER_indcpa.o verify.o 

CFLAGS=-O3 -mavx2 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

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

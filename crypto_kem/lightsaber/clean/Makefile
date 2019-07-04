# This Makefile can be used with GNU Make or BSD Make

LIB=liblightsaber_clean.a
HEADERS=api.h cbd.h poly.h poly_mul.h SABER_indcpa.h SABER_params.h verify.h pack_unpack.h 
OBJECTS=cbd.o kem.o pack_unpack.o poly.o poly_mul.o SABER_indcpa.o verify.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

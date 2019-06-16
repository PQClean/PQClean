# This Makefile can be used with GNU Make or BSD Make

LIB=libledakemlt32_leaktime.a
HEADERS=api.h bf_decoding.h dfr_test.h gf2x_arith_mod_xPplusOne.h \
		gf2x_arith.h H_Q_matrices_generation.h \
		niederreiter.h qc_ldpc_parameters.h rng.h

OBJECTS=bf_decoding.o dfr_test.o gf2x_arith_mod_xPplusOne.o \
		gf2x_arith.o H_Q_matrices_generation.o kem.o niederreiter.o rng.o

CFLAGS=-O3 -Wall -Werror -Wextra -Wvla -Wpedantic -Wmissing-prototypes -std=c99 \
		-I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

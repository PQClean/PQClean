# This Makefile can be used with GNU Make or BSD Make

LIB = libledacrypt23371_clean.a

HEADERS = H_Q_matrices_generation.h djbsort.h gf2x_limbs.h \
      gf2x_arith.h rng.h gf2x_arith_mod_xPplusOne.h \
      api.h niederreiter.h niederreiter_keygen.h bf_decoding.h niederreiter_decrypt.h \
      qc_ldpc_parameters.h dfr_test.h niederreiter_encrypt.h \
      aes256.h

OBJECTS = aes256.o bf_decoding.o dfr_test.o djbsort.o gf2x_arith.o gf2x_arith_mod_xPplusOne.o \
	  H_Q_matrices_generation.o kem.o niederreiter_decrypt.o niederreiter_encrypt.o \
	  niederreiter_keygen.o rng.o

CFLAGS  = -O3 -std=c99 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes \
      -march=native -g3 -I../../../common

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
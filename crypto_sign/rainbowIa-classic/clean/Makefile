# This Makefile can be used with GNU Make or BSD Make

LIB=librainbowIa-classic_clean.a

HEADERS = api.h blas_comm.h blas.h gf.h parallel_matrix_op.h rainbow_blas.h rainbow_config.h rainbow.h rainbow_keypair_computation.h rainbow_keypair.h utils_hash.h utils_prng.h 
OBJECTS =  blas_comm.o parallel_matrix_op.o rainbow.o rainbow_keypair.o rainbow_keypair_computation.o sign.o utils_hash.o utils_prng.o blas.o gf.o


CFLAGS=-O3 -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

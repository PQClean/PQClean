# This Makefile can be used with GNU Make or BSD Make

LIB=libntruhps2048677_clean.a
HEADERS=api.h crypto_sort.h owcpa.h params.h poly.h sample.h verify.h
OBJECTS=crypto_sort.o kem.o owcpa.o pack3.o packq.o poly.o poly_lift.o poly_mod.o poly_r2_inv.o poly_rq_mul.o poly_s3_inv.o sample.o sample_iid.o verify.o

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

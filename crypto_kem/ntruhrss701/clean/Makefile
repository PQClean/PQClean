# This Makefile can be used with GNU Make or BSD Make

LIB=libntruhrss701_clean.a
HEADERS=api.h cmov.h owcpa.h params.h poly.h sample.h 
OBJECTS=cmov.o kem.o owcpa.o pack3.o packq.o poly.o poly_lift.o poly_mod.o poly_r2_inv.o poly_rq_mul.o poly_s3_inv.o sample.o sample_iid.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

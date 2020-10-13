# This Makefile can be used with GNU Make or BSD Make

LIB=libntruhrss701_avx2.a
HEADERS=api.h cmov.h owcpa.h params.h poly.h poly_r2_inv.h sample.h 
OBJECTS=cmov.o kem.o owcpa.o pack3.o packq.o poly.o poly_r2_inv.o poly_s3_inv.o sample.o sample_iid.o  \
        square_1_701_patience.o square_3_701_patience.o square_6_701_patience.o square_12_701_shufbytes.o square_15_701_shufbytes.o square_27_701_shufbytes.o square_42_701_shufbytes.o square_84_701_shufbytes.o square_168_701_shufbytes.o square_336_701_shufbytes.o  \
        poly_lift.o poly_mod_3_Phi_n.o poly_mod_q_Phi_n.o poly_r2_mul.o poly_rq_mul.o poly_rq_to_s3.o  vec32_sample_iid.o

CFLAGS=-O3 -mavx2 -mbmi2 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

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

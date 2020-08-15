# This Makefile can be used with GNU Make or BSD Make

LIB=libntruhps2048677_avx2.a
HEADERS=api.h crypto_int8.h crypto_sort.h owcpa.h params.h poly.h poly_r2_inv.h sample.h verify.h
OBJECTS=crypto_sort.o kem.o owcpa.o pack3.o packq.o poly.o poly_lift.o poly_r2_inv.o poly_rq_mul.o poly_s3_inv.o sample.o sample_iid.o verify.o \
        square_1_677_patience.o square_2_677_patience.o square_3_677_patience.o square_5_677_patience.o square_10_677_shufbytes.o square_21_677_shufbytes.o square_42_677_shufbytes.o square_84_677_shufbytes.o square_168_677_shufbytes.o square_336_677_shufbytes.o \
        poly_rq_mul.o poly_r2_mul.o poly_rq_to_s3.o vec32_sample_iid.o poly_mod_3_Phi_n.o poly_mod_q_Phi_n.o

CFLAGS=-O3 -mavx2 -mbmi2 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

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

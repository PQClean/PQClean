# This Makefile can be used with GNU Make or BSD Make

LIB=libntruhps4096821_avx2.a
HEADERS=api.h cmov.h crypto_sort_int32.h owcpa.h params.h poly.h poly_r2_inv.h sample.h 
OBJECTS=cmov.o crypto_sort_int32.o kem.o owcpa.o pack3.o packq.o poly.o poly_lift.o poly_r2_inv.o poly_s3_inv.o sample.o sample_iid.o  \
        square_1_821_patience.o square_3_821_patience.o square_6_821_patience.o square_12_821_shufbytes.o square_24_821_shufbytes.o square_51_821_shufbytes.o square_102_821_shufbytes.o square_204_821_shufbytes.o square_408_821_shufbytes.o  \
        poly_mod_3_Phi_n.o poly_mod_q_Phi_n.o poly_r2_mul.o poly_rq_mul.o poly_rq_to_s3.o  vec32_sample_iid.o

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

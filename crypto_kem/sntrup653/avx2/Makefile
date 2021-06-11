# This Makefile can be used with GNU Make or BSD Make

LIB=libsntrup653_avx2.a
HEADERS=api.h crypto_core_inv3sntrup653.h crypto_core_invsntrup653.h crypto_core_mult3sntrup653.h crypto_core_multsntrup653.h crypto_core_multsntrup653_ntt.h crypto_core_scale3sntrup653.h crypto_core_weightsntrup653.h crypto_core_wforcesntrup653.h crypto_declassify.h crypto_decode_653x1541.h crypto_decode_653x3.h crypto_decode_653x4621.h crypto_decode_653xint16.h crypto_decode_653xint32.h crypto_decode_int16.h crypto_encode_653x1541.h crypto_encode_653x1541round.h crypto_encode_653x3.h crypto_encode_653x4621.h crypto_encode_653xfreeze3.h crypto_encode_653xint16.h crypto_encode_int16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_verify_897.h params.h 
OBJECTS=crypto_core_inv3sntrup653.o crypto_core_invsntrup653.o crypto_core_mult3sntrup653.o crypto_core_multsntrup653.o crypto_core_multsntrup653_ntt.o crypto_core_scale3sntrup653.o crypto_core_weightsntrup653.o crypto_core_wforcesntrup653.o crypto_decode_653x1541.o crypto_decode_653x3.o crypto_decode_653x4621.o crypto_decode_653xint16.o crypto_decode_653xint32.o crypto_decode_int16.o crypto_encode_653x1541.o crypto_encode_653x1541round.o crypto_encode_653x3.o crypto_encode_653x4621.o crypto_encode_653xfreeze3.o crypto_encode_653xint16.o crypto_encode_int16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_verify_897.o kem.o 

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

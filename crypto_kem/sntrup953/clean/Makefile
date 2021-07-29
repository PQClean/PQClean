# This Makefile can be used with GNU Make or BSD Make

LIB=libsntrup953_clean.a
HEADERS=api.h crypto_core_inv3sntrup953.h crypto_core_invsntrup953.h crypto_core_mult3sntrup953.h crypto_core_multsntrup953.h crypto_core_scale3sntrup953.h crypto_core_weightsntrup953.h crypto_core_wforcesntrup953.h crypto_declassify.h crypto_decode_953x2115.h crypto_decode_953x3.h crypto_decode_953x6343.h crypto_decode_953xint16.h crypto_decode_953xint32.h crypto_encode_953x2115.h crypto_encode_953x2115round.h crypto_encode_953x3.h crypto_encode_953x6343.h crypto_encode_953xfreeze3.h crypto_encode_953xint16.h crypto_encode_int16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_verify_1349.h params.h 
OBJECTS=crypto_core_inv3sntrup953.o crypto_core_invsntrup953.o crypto_core_mult3sntrup953.o crypto_core_multsntrup953.o crypto_core_scale3sntrup953.o crypto_core_weightsntrup953.o crypto_core_wforcesntrup953.o crypto_decode_953x2115.o crypto_decode_953x3.o crypto_decode_953x6343.o crypto_decode_953xint16.o crypto_decode_953xint32.o crypto_encode_953x2115.o crypto_encode_953x2115round.o crypto_encode_953x3.o crypto_encode_953x6343.o crypto_encode_953xfreeze3.o crypto_encode_953xint16.o crypto_encode_int16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_verify_1349.o kem.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

# This Makefile can be used with GNU Make or BSD Make

LIB=libsntrup1013_clean.a
HEADERS=api.h crypto_core_inv3sntrup1013.h crypto_core_invsntrup1013.h crypto_core_mult3sntrup1013.h crypto_core_multsntrup1013.h crypto_core_scale3sntrup1013.h crypto_core_weightsntrup1013.h crypto_core_wforcesntrup1013.h crypto_declassify.h crypto_decode_1013x2393.h crypto_decode_1013x3.h crypto_decode_1013x7177.h crypto_decode_1013xint16.h crypto_decode_1013xint32.h crypto_encode_1013x2393.h crypto_encode_1013x2393round.h crypto_encode_1013x3.h crypto_encode_1013x7177.h crypto_encode_1013xfreeze3.h crypto_encode_1013xint16.h crypto_encode_int16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_verify_1455.h params.h 
OBJECTS=crypto_core_inv3sntrup1013.o crypto_core_invsntrup1013.o crypto_core_mult3sntrup1013.o crypto_core_multsntrup1013.o crypto_core_scale3sntrup1013.o crypto_core_weightsntrup1013.o crypto_core_wforcesntrup1013.o crypto_decode_1013x2393.o crypto_decode_1013x3.o crypto_decode_1013x7177.o crypto_decode_1013xint16.o crypto_decode_1013xint32.o crypto_encode_1013x2393.o crypto_encode_1013x2393round.o crypto_encode_1013x3.o crypto_encode_1013x7177.o crypto_encode_1013xfreeze3.o crypto_encode_1013xint16.o crypto_encode_int16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_verify_1455.o kem.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

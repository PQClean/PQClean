# This Makefile can be used with GNU Make or BSD Make

LIB=libsntrup1277_clean.a
HEADERS=api.h crypto_core_inv3sntrup1277.h crypto_core_invsntrup1277.h crypto_core_mult3sntrup1277.h crypto_core_multsntrup1277.h crypto_core_scale3sntrup1277.h crypto_core_weightsntrup1277.h crypto_core_wforcesntrup1277.h crypto_declassify.h crypto_decode_1277x2627.h crypto_decode_1277x3.h crypto_decode_1277x7879.h crypto_decode_1277xint16.h crypto_decode_1277xint32.h crypto_encode_1277x2627.h crypto_encode_1277x2627round.h crypto_encode_1277x3.h crypto_encode_1277x7879.h crypto_encode_1277xfreeze3.h crypto_encode_1277xint16.h crypto_encode_int16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_verify_1847.h params.h 
OBJECTS=crypto_core_inv3sntrup1277.o crypto_core_invsntrup1277.o crypto_core_mult3sntrup1277.o crypto_core_multsntrup1277.o crypto_core_scale3sntrup1277.o crypto_core_weightsntrup1277.o crypto_core_wforcesntrup1277.o crypto_decode_1277x2627.o crypto_decode_1277x3.o crypto_decode_1277x7879.o crypto_decode_1277xint16.o crypto_decode_1277xint32.o crypto_encode_1277x2627.o crypto_encode_1277x2627round.o crypto_encode_1277x3.o crypto_encode_1277x7879.o crypto_encode_1277xfreeze3.o crypto_encode_1277xint16.o crypto_encode_int16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_verify_1847.o kem.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

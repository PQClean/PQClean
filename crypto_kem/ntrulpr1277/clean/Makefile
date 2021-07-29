# This Makefile can be used with GNU Make or BSD Make

LIB=libntrulpr1277_clean.a
HEADERS=api.h crypto_core_multsntrup1277.h crypto_declassify.h crypto_decode_1277x2627.h crypto_decode_1277x3.h crypto_decode_1277xint16.h crypto_decode_1277xint32.h crypto_decode_256x16.h crypto_decode_256x2.h crypto_encode_1277x2627.h crypto_encode_1277x2627round.h crypto_encode_1277x3.h crypto_encode_1277xint16.h crypto_encode_256x16.h crypto_encode_256x2.h crypto_sort_int32.h crypto_sort_uint32.h crypto_stream_aes256ctr.h crypto_verify_1975.h params.h 
OBJECTS=crypto_core_multsntrup1277.o crypto_decode_1277x2627.o crypto_decode_1277x3.o crypto_decode_1277xint16.o crypto_decode_1277xint32.o crypto_decode_256x16.o crypto_decode_256x2.o crypto_encode_1277x2627.o crypto_encode_1277x2627round.o crypto_encode_1277x3.o crypto_encode_1277xint16.o crypto_encode_256x16.o crypto_encode_256x2.o crypto_sort_int32.o crypto_sort_uint32.o crypto_stream_aes256ctr.o crypto_verify_1975.o kem.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

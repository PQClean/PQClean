# This Makefile can be used with GNU Make or BSD Make

LIB=libsntrup857_clean.a
HEADERS=api.h crypto_core_inv3sntrup857.h crypto_core_invsntrup857.h crypto_core_mult3sntrup857.h crypto_core_multsntrup857.h crypto_core_scale3sntrup857.h crypto_core_weightsntrup857.h crypto_core_wforcesntrup857.h crypto_decode_857x1723.h crypto_decode_857x3.h crypto_decode_857x5167.h crypto_decode_857xint16.h crypto_decode_857xint32.h crypto_encode_857x1723.h crypto_encode_857x1723round.h crypto_encode_857x3.h crypto_encode_857x5167.h crypto_encode_857xfreeze3.h crypto_encode_857xint16.h crypto_encode_int16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_stream_aes256ctr.h crypto_verify_1184.h params.h 
OBJECTS=crypto_core_inv3sntrup857.o crypto_core_invsntrup857.o crypto_core_mult3sntrup857.o crypto_core_multsntrup857.o crypto_core_scale3sntrup857.o crypto_core_weightsntrup857.o crypto_core_wforcesntrup857.o crypto_decode_857x1723.o crypto_decode_857x3.o crypto_decode_857x5167.o crypto_decode_857xint16.o crypto_decode_857xint32.o crypto_encode_857x1723.o crypto_encode_857x1723round.o crypto_encode_857x3.o crypto_encode_857x5167.o crypto_encode_857xfreeze3.o crypto_encode_857xint16.o crypto_encode_int16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_stream_aes256ctr.o crypto_verify_1184.o kem.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

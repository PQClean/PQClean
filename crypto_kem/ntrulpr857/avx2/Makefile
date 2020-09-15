# This Makefile can be used with GNU Make or BSD Make

LIB=libntrulpr857_avx2.a
HEADERS=api.h crypto_core_multsntrup857.h crypto_core_multsntrup857_ntt.h crypto_decode_256x16.h crypto_decode_256x2.h crypto_decode_857x1723.h crypto_decode_857x3.h crypto_decode_857xint16.h crypto_decode_857xint32.h crypto_encode_256x16.h crypto_encode_256x2.h crypto_encode_857x1723.h crypto_encode_857x1723round.h crypto_encode_857x3.h crypto_encode_857xint16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_stream_aes256ctr.h crypto_verify_1312.h params.h 
OBJECTS=crypto_core_multsntrup857.o crypto_core_multsntrup857_ntt.o crypto_decode_256x16.o crypto_decode_256x2.o crypto_decode_857x1723.o crypto_decode_857x3.o crypto_decode_857xint16.o crypto_decode_857xint32.o crypto_encode_256x16.o crypto_encode_256x2.o crypto_encode_857x1723.o crypto_encode_857x1723round.o crypto_encode_857x3.o crypto_encode_857xint16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_stream_aes256ctr.o crypto_verify_1312.o kem.o 

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

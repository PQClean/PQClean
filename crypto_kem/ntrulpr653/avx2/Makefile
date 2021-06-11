# This Makefile can be used with GNU Make or BSD Make

LIB=libntrulpr653_avx2.a
HEADERS=api.h crypto_core_multsntrup653.h crypto_core_multsntrup653_ntt.h crypto_declassify.h crypto_decode_256x16.h crypto_decode_256x2.h crypto_decode_653x1541.h crypto_decode_653x3.h crypto_decode_653xint16.h crypto_decode_653xint32.h crypto_encode_256x16.h crypto_encode_256x2.h crypto_encode_653x1541.h crypto_encode_653x1541round.h crypto_encode_653x3.h crypto_encode_653xint16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_stream_aes256ctr.h crypto_verify_1025.h params.h 
OBJECTS=crypto_core_multsntrup653.o crypto_core_multsntrup653_ntt.o crypto_decode_256x16.o crypto_decode_256x2.o crypto_decode_653x1541.o crypto_decode_653x3.o crypto_decode_653xint16.o crypto_decode_653xint32.o crypto_encode_256x16.o crypto_encode_256x2.o crypto_encode_653x1541.o crypto_encode_653x1541round.o crypto_encode_653x3.o crypto_encode_653xint16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_stream_aes256ctr.o crypto_verify_1025.o kem.o 

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

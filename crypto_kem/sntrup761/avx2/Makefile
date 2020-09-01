# This Makefile can be used with GNU Make or BSD Make

LIB=libsntrup761_avx2.a
HEADERS=api.h crypto_core_inv3sntrup761.h crypto_core_invsntrup761.h crypto_core_mult3sntrup761.h crypto_core_multsntrup761.h crypto_core_multsntrup761_ntt.h crypto_core_scale3sntrup761.h crypto_core_weightsntrup761.h crypto_core_wforcesntrup761.h crypto_decode_761x1531.h crypto_decode_761x3.h crypto_decode_761x4591.h crypto_decode_761xint16.h crypto_decode_761xint32.h crypto_decode_int16.h crypto_encode_761x1531.h crypto_encode_761x1531round.h crypto_encode_761x3.h crypto_encode_761x4591.h crypto_encode_761xfreeze3.h crypto_encode_761xint16.h crypto_encode_int16.h crypto_sort_int32.h crypto_sort_uint32.h crypto_stream_aes256ctr.h crypto_verify_1039.h params.h 
OBJECTS=crypto_core_inv3sntrup761.o crypto_core_invsntrup761.o crypto_core_mult3sntrup761.o crypto_core_multsntrup761.o crypto_core_multsntrup761_ntt.o crypto_core_scale3sntrup761.o crypto_core_weightsntrup761.o crypto_core_wforcesntrup761.o crypto_decode_761x1531.o crypto_decode_761x3.o crypto_decode_761x4591.o crypto_decode_761xint16.o crypto_decode_761xint32.o crypto_decode_int16.o crypto_encode_761x1531.o crypto_encode_761x1531round.o crypto_encode_761x3.o crypto_encode_761x4591.o crypto_encode_761xfreeze3.o crypto_encode_761xint16.o crypto_encode_int16.o crypto_sort_int32.o crypto_sort_uint32.o crypto_stream_aes256ctr.o crypto_verify_1039.o kem.o 

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

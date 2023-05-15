# This Makefile can be used with GNU Make or BSD Make

LIB = libmceliece6960119_avx2.a

SOURCES = aes256ctr.c benes.c bm.c consts.S controlbits.c crypto_int16.c crypto_int32.c \
		crypto_uint16.c crypto_uint32.c crypto_uint64.c decrypt.c encrypt.c fft.c \
		fft_tr.c gf.c operations.c pk_gen.c sk_gen.c syndrome_asm.S \
		transpose_64x128_sp_asm.S transpose_64x256_sp_asm.S transpose_64x64_asm.S \
		update_asm.S vec128.c vec128_mul_asm.S vec256.c vec256_ama_asm.S \
		vec256_maa_asm.S vec256_mul_asm.S vec_reduce_asm.S

HEADERS = aes256ctr.h api.h api.h benes.h bm.h controlbits.h crypto_hash.h crypto_hash.h \
		crypto_int16.h crypto_int32.h crypto_kem.h crypto_uint16.h crypto_uint32.h \
		crypto_uint64.h decrypt.h encrypt.h fft.h fft_tr.h gf.h int32_sort.h \
		int32_sort.h namespace.h operations.h params.h pk_gen.h sk_gen.h transpose.h \
		uint64_sort.h util.h vec128.h vec256.h

OBJECTS = aes256ctr.o benes.o bm.o consts.o controlbits.o crypto_int16.o crypto_int32.o \
		crypto_uint16.o crypto_uint32.o crypto_uint64.o decrypt.o encrypt.o fft.o \
		fft_tr.o gf.o operations.o pk_gen.o sk_gen.o syndrome_asm.o \
		transpose_64x128_sp_asm.o transpose_64x256_sp_asm.o transpose_64x64_asm.o \
		update_asm.o vec128.o vec128_mul_asm.o vec256.o vec256_ama_asm.o \
		vec256_maa_asm.o vec256_mul_asm.o vec_reduce_asm.o


CFLAGS  = -O3 -std=c99 -mpopcnt -mavx2 -Wall -Wextra -pedantic -Werror -Wpedantic \
		  -fwrapv -Wredundant-decls -Wcast-align -Wmissing-prototypes \
		  -I../../../common/ $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS)   -c -o $@ $<

%.o: %.S
	$(CC) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
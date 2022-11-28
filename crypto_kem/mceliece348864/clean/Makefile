# This Makefile can be used with GNU Make or BSD Make

LIB = libmceliece348864_clean.a

SOURCES = aes256ctr.c benes.c bm.c controlbits.c crypto_int16.c crypto_int32.c \
		crypto_uint16.c crypto_uint32.c crypto_uint64.c decrypt.c encrypt.c gf.c \
		operations.c pk_gen.c root.c sk_gen.c synd.c transpose.c util.c

HEADERS = aes256ctr.h api.h api.h benes.h bm.h controlbits.h crypto_hash.h crypto_hash.h \
		crypto_int16.h crypto_int32.h crypto_kem.h crypto_uint16.h crypto_uint32.h \
		crypto_uint64.h decrypt.h encrypt.h gf.h int32_sort.h int32_sort.h namespace.h \
		operations.h params.h pk_gen.h root.h sk_gen.h synd.h transpose.h uint64_sort.h \
		util.h

OBJECTS = aes256ctr.o benes.o bm.o controlbits.o crypto_int16.o crypto_int32.o \
		crypto_uint16.o crypto_uint32.o crypto_uint64.o decrypt.o encrypt.o gf.o \
		operations.o pk_gen.o root.o sk_gen.o synd.o transpose.o util.o

CFLAGS  = -O3 -std=c99 -Wall -Wextra -pedantic -Werror -Wpedantic \
	  -Wredundant-decls -Wcast-align -Wmissing-prototypes \
	  -I../../../common/ $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS)   -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
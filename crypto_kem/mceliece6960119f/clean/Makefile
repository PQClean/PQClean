# This Makefile can be used with GNU Make or BSD Make

LIB = libmceliece6960119f_clean.a

SOURCES = aes256ctr.c benes.c bm.c controlbits.c decrypt.c encrypt.c gf.c \
	  operations.c pk_gen.c root.c sk_gen.c synd.c transpose.c util.c
HEADERS = aes256ctr.h api.h benes.h bm.h controlbits.h crypto_hash.h \
	  decrypt.h encrypt.h gf.h params.h pk_gen.h root.h \
	  sk_gen.h synd.h transpose.h util.h
OBJECTS = aes256ctr.o benes.o bm.o controlbits.o decrypt.o encrypt.o gf.o \
	  operations.o pk_gen.o root.o sk_gen.o synd.o transpose.o util.o

CFLAGS  = -O3 -std=c99 -Wall -Wextra -pedantic -Werror -Wpedantic \
	  -Wredundant-decls -Wvla -Wcast-align -Wmissing-prototypes \
	  -I../../../common/ $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS)   -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

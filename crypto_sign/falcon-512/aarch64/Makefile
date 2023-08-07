# This Makefile can be used with GNU Make or BSD Make

LIB=libfalcon-512_aarch64.a

SOURCES = codec.c keygen.c poly_float.c common.c fft_tree.c pqclean.c poly_int.c sign.c fpr.c ntt.c rng.c util.c fft.c ntt_consts.c sampler.c vrfy.c
OBJECTS = codec.o keygen.o poly_float.o common.o fft_tree.o pqclean.o poly_int.o sign.o fpr.o ntt.o rng.o util.o fft.o ntt_consts.o sampler.o vrfy.o
HEADERS = api.h fpr.h inner.h macrof.h macrofx4.h macrous.h ntt_consts.h params.h poly.h util.h 

CFLAGS=-std=c99 -O3 -W -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

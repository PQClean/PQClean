# This Makefile can be used with GNU Make or BSD Make

LIB=libntruhps2048509_clean.a
HEADERS=api.h crypto_sort.h owcpa.h params.h poly.h sample.h verify.h
OBJECTS=crypto_sort.o kem.o owcpa.o pack3.o packq.o poly.o sample.o verify.o

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

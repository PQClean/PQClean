# This Makefile can be used with GNU Make or BSD Make

LIB=libnewhope1024cca_clean.a
HEADERS=api.h cpapke.h ntt.h params.h poly.h reduce.h verify.h
OBJECTS=cpapke.o kem.o ntt.o poly.o precomp.o reduce.o verify.o

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

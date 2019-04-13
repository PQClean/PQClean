# This Makefile can be used with GNU Make or BSD Make

LIB=libfrodokem640aes_clean.a
HEADERS=api.h params.h common.h
OBJECTS=kem.o matrix_aes.o noise.o util.o

CFLAGS=-Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

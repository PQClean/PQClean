# This Makefile can be used with GNU Make or BSD Make

LIB=libntrulpr857_clean.a
HEADERS=api.h decode.h encode.h int32.h params.h uint32.h
OBJECTS=decode.o encode.o int32.o kem.o uint32_sort.o uint32.o

CFLAGS=-Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

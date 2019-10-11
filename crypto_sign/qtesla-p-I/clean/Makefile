# This Makefile can be used with GNU Make or BSD Make

LIB=libqtesla-p-I_clean.a

HEADERS = api.h CDT32.h config.h gauss.h pack.h params.h poly.h sample.h
OBJECTS = consts.o gauss.o pack.o poly.o sample.o sign.o

CFLAGS=-O3 -Wall -Wconversion -Wextra -Wpedantic -Wvla -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

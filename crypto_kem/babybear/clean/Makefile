# This Makefile can be used with GNU Make or BSD Make

LIB=libbabybear_clean.a

HEADERS = api.h melas_fec.h params.h ring.h threebears.h
OBJECTS = kem.o melas_fec.o ring.o threebears.o 


CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

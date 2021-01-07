# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium5-AES-R_avx2.a

SOURCES = aes256ctr.c invntt.S consts.c ntt.S packing.c pointwise.S poly.c \
		  polyvec.c rejsample.c rounding.c sign.c symmetric-aes.c
OBJECTS = aes256ctr.o invntt.o consts.o ntt.o packing.o pointwise.o poly.o \
		  polyvec.o rejsample.o rounding.o sign.o symmetric-aes.o
HEADERS = aes256ctr.h alignment.h api.h params.h sign.h polyvec.h poly.h \
		  packing.h ntt.h consts.h rounding.h rejsample.h symmetric.h \
		  alignment.h shuffle.inc cdecl.inc

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror \
	   -Wmissing-prototypes -Wredundant-decls -std=c99 \
	   -Wcast-align -Werror=shadow\
	   -mavx2 -mbmi -mpopcnt -maes -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $^


clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

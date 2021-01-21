# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium2aes_avx2.a
HEADERS=aes256ctr.h align.h api.h cdecl.h consts.h ntt.h packing.h params.h poly.h polyvec.h rejsample.h rounding.h sign.h symmetric.h  shuffle.inc 
OBJECTS=aes256ctr.o consts.o packing.o poly.o polyvec.o rejsample.o rounding.o sign.o  invntt.o ntt.o pointwise.o shuffle.o 
CFLAGS=-mavx2 -maes -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror \
          -Wmissing-prototypes -Wredundant-decls -std=c99 \
          -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

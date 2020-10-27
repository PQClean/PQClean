# This Makefile can be used with GNU Make or BSD Make

LIB=libkyber768-90s_avx2.a
HEADERS=aes256ctr.h align.h api.h cbd.h cdecl.h consts.h indcpa.h kem.h ntt.h params.h poly.h polyvec.h reduce.h rejsample.h symmetric.h verify.h  fq.inc shuffle.inc 
OBJECTS=aes256ctr.o cbd.o consts.o indcpa.o kem.o poly.o polyvec.o rejsample.o verify.o  basemul.o fq.o invntt.o ntt.o shuffle.o 
CFLAGS=-mavx2 -maes -mbmi2 -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror \
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

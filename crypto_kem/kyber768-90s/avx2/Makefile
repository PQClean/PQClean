# This Makefile can be used with GNU Make or BSD Make

LIB=libkyber768-90s_avx2.a
HEADERS=api.h params.h poly.h polyvec.h reduce.h fq.inc cbd.h consts.h ntt.h shuffle.inc verify.h indcpa.h rejsample.h symmetric.h aes256ctr.h
OBJECTS=kem.o poly.o polyvec.o fq.o shuffle.o cbd.o ntt.o invntt.o basemul.o consts.o \
		verify.o indcpa.o rejsample.o aes256ctr.o

CFLAGS=-mavx2 -maes -mbmi2 -mpopcnt -O3 -Wall -Wextra -Wpedantic -Werror \
	   -Wmissing-prototypes -Wredundant-decls -std=c99 \
	   -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s $(HEADERS)
	$(AS) -o $@ $<

%.o: %.S $(HEADERS)
	$(AS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

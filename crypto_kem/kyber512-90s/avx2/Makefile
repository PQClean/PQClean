# This Makefile can be used with GNU Make or BSD Make

LIB=libkyber512-90s_avx2.a
HEADERS=		   \
	aes256ctr.h	   \
	align.h        \
	api.h          \
	cbd.h          \
	cdecl.inc      \
	consts.h       \
	fq.inc         \
	indcpa.h       \
	kem.h          \
	ntt.h          \
	params.h       \
	poly.h         \
	polyvec.h      \
	reduce.h       \
	rejsample.h    \
	shuffle.inc    \
	symmetric.h    \
	verify.h
OBJECTS=		   \
	aes256ctr.o    \
	basemul.o      \
	cbd.o          \
	consts.o       \
	fq.o           \
	indcpa.o       \
	invntt.o       \
	kem.o          \
	ntt.o          \
	poly.o         \
	polyvec.o      \
	rejsample.o    \
	shuffle.o      \
	verify.o

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

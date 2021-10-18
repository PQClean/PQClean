# This Makefile can be used with GNU Make or BSD Make

LIB=libsaber_aarch64.a
HEADERS=api.h cbd.h fips202x2.h kem.h macros_common.inc macros.inc NTT_params.h NTT.h pack_unpack.h SABER_indcpa.h SABER_params.h verify.h
OBJECTS=cbd.o fips202x2.o kem.o pack_unpack.o SABER_indcpa.o verify.o __asm_iNTT.o __asm_mul.o __asm_narrow.o __asm_NTT.o __asm_pack_unpack.o


CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

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

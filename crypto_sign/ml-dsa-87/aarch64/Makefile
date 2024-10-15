# This Makefile can be used with GNU Make or BSD Make

LIB=libml-dsa-87_aarch64.a
HEADERS=api.h macros_common.inc macros.inc NTT_params.h ntt.h packing.h params.h poly.h polyvec.h reduce.h rounding.h sign.h symmetric.h
OBJECTS= ntt.o packing.o poly.o polyvec.o reduce.o rounding.o sign.o symmetric-shake.o __asm_iNTT.o __asm_NTT.o __asm_poly.o


CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS) -g

KECCAK2XDIR=../../../common/keccak2x
KECCAK2XOBJ=fips202x2.o feat.o
KECCAK2X=$(addprefix $(KECCAK2XDIR)/,$(KECCAK2XOBJ))

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) $(KECCAK2X)
	$(AR) -r $@ $(OBJECTS) $(KECCAK2X)

$(KECCAK2X):
	$(MAKE) -C $(KECCAK2XDIR) CFLAGS="$(CFLAGS)" $(KECCAK2XOBJ)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)

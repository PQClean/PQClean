# This Makefile can be used with GNU Make or BSD Make

SL = 1
N0 = 2
DFR_SL_LEVEL = 1

LIB=libLEDAkem_sl$(SL)_N0$(N0)_clean.a
HEADERS=aes256.h api.h bf_decoding.h dfr_test.h gf2x_arith_mod_xPplusOne.h \
		gf2x_arith.h gf2x_limbs.h H_Q_matrices_generation.h niederreiter_decrypt.h \
		niederreiter_encrypt.h niederreiter_keygen.h niederreiter.h \
		qc_ldpc_parameters.h rng.h sha3.h

OBJECTS=aes256.o bf_decoding.o dfr_test.o gf2x_arith_mod_xPplusOne.o \
		gf2x_arith.o H_Q_matrices_generation.o kem.o niederreiter_decrypt.o \
		niederreiter_encrypt.o niederreiter_keygen.o rng.o

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Wmissing-prototypes -std=c99 \
		-DCATEGORY=$(SL) -DN0=$(N0) -DDFR_SL_LEVEL=$(DFR_SL_LEVEL) -I../../../common $(EXTRAFLAGS)

# TODO: -Werror

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)


# old makefile
# CFLAGS = -DCATEGORY=$(SL) -DN0=$(N0) -DDFR_SL_LEVEL=$(DFR_SL_LEVEL) -DCPU_WORD_BITS=64 \
#        -std=c11 -Wall -pedantic -Wmaybe-uninitialized -Wuninitialized \
#    -march=native -O3 -g3
# LDFLAGS = -lm -lkeccak
# INCLUDES = -I./include
# SRCDIR = library
# OBJDIR = bin
#
# # Gathers the names of all C files
#     CSRC = $(wildcard $(SRCDIR)/*.c)
# # Produces in $(COBJS) the names of .o object files for all C files
#    COBJS = $(CSRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
#
# $(OBJDIR)/%.o: $(SRCDIR)/%.c
# 	$(CC) -c $(CFLAGS) $(INCLUDES)  $< -o $@
#
# .PHONY : all clean
#
# all: $(COBJS)
# 	ar rcs $(OBJDIR)/libLEDAkem_sl$(SL)_N0$(N0).a $(COBJS)
# clean:
# 	$(RM) $(OBJDIR)/*.o  $(OBJDIR)/libLEDAkem_sl$(SL)_N0$(N0).a

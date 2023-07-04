# This Makefile compiles the implementation in this directory along with
# the known answer tests generator located in the
# ../../../KAT/generator/ directory. The output is an executable program
# in the build/ subdirectory, whose name starts with 'kat', followed by
# the implementation name (e.g. 'kat512int' for the 'falcon512int'
# implementation). This program, when executed, generates the .req and
# .rsp files in the expected NIST format.

.POSIX:

# CC = gcc
CC = clang
CFLAGS = -fomit-frame-pointer -W -Wall -O3 -Wextra -Wpedantic -Wshadow -Wundef -Wno-overlength-strings
LD = clang
LDFLAGS = 
LIBS =

OBJ = codec.c util.c common.c fft.c fft_tree.c \
	  fpr.c keygen.c rng.c katrng.c poly_float.c sampler.c  shake.c \
	  sign.c vrfy.c ntt.c ntt_consts.c poly_int.c nist.c

OBJ_SPEED = falcon.c speed.c
OBJ_SPEED_Ghz = falcon.c speed_freq.c
OBJ_BENCH = bench.c
OBJ_KAT = PQCgenKAT_sign.c
OBJ_TEST_FALCON = falcon.c test_falcon.c

HEAD = api.h fpr.h inner.h config.h katrng.h params.h macrous.h macrof.h macrofx4.h
HEAD1 = falcon.h

all: build kat
kat: build/kat512fpu build/kat1024fpu
m1_test: build/m1_test_falcon512 build/m1_test_falcon1024
m1: build/m1_speed512 build/m1_speed1024 build/m1_bench512 build/m1_bench1024
m1_59b: build/m1_speed_59b_512 build/m1_speed_59b_1024
m1_ghz: build/m1_speed512_ghz build/m1_speed1024_ghz
a72_test: build/a72_test_falcon512 build/a72_test_falcon1024
a72: build/a72_speed512 build/a72_speed1024 build/a72_bench512 build/a72_bench1024
a72_59b: build/a72_speed_59b_512 build/a72_speed_59b_1024
a72_ghz: build/a72_speed512_ghz build/a72_speed1024_ghz


build:
	-mkdir build

clean:
	-rm -f build/kat512fpu build/kat1024fpu
	-rm -f build/a72_test_falcon512 build/a72_test_falcon1024
	-rm -f build/a72_speed512 build/a72_speed1024 build/a72_bench512 build/a72_bench1024
	-rm -f build/a72_speed512_ghz build/a72_speed1024_ghz
	-rm -f build/m1_test_falcon512 build/m1_test_falcon1024
	-rm -f build/m1_speed512 build/m1_speed1024 build/m1_bench512 build/m1_bench1024
	-rm -f build/m1_speed512_ghz build/m1_speed1024_ghz
	-rm -f build/a72_speed_59b_512 build/a72_speed_59b_1024
	-rm -f build/m1_speed_59b_512 build/m1_speed_59b_1024

build/kat512fpu: $(HEAD) $(OBJ)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DALGNAME=falcon512fpu  -o $@ $(OBJ) $(OBJ_KAT)
	$@

build/kat1024fpu: $(HEAD) $(OBJ)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DALGNAME=falcon1024fpu -o $@ $(OBJ) $(OBJ_KAT)
	$@

################### APPLE M1 ###################

build/m1_speed512: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) $(OBJ_SPEED)
	sudo $@

build/m1_speed1024: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) $(OBJ_SPEED)
	sudo $@

build/m1_bench512: $(OBJ) $(OBJ_BENCH) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) $(OBJ_BENCH)
	sudo $@

build/m1_bench1024: $(OBJ) $(OBJ_BENCH) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) $(OBJ_BENCH)
	sudo $@

build/m1_test_falcon512: $(OBJ) $(OBJ_TEST_FALCON) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9 -DAPPLE_M1=1 -o $@ $(OBJ) $(OBJ_TEST_FALCON)
	$@

build/m1_test_falcon1024: $(OBJ) $(OBJ_TEST_FALCON) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=1 -o $@ $(OBJ) $(OBJ_TEST_FALCON)
	$@

build/m1_speed512_ghz: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED_Ghz)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) $(OBJ_SPEED_Ghz)
	sudo $@

build/m1_speed1024_ghz: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED_Ghz)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) $(OBJ_SPEED_Ghz)
	sudo $@

build/m1_speed_59b_512: $(HEAD1) $(HEAD) $(OBJ)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) speed_59b_512.c
	sudo $@

build/m1_speed_59b_1024: $(HEAD1) $(HEAD) $(OBJ)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=1 -DBENCH_CYCLES=1 -o $@ m1cycles.c $(OBJ) speed_59b_1024.c
	sudo $@
################### A72 ###################

build/a72_speed512: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) $(OBJ_SPEED)
	$@

build/a72_speed1024: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) $(OBJ_SPEED)
	$@

build/a72_bench512: $(OBJ) $(OBJ_BENCH) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DBENCH_CYCLES=1 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) $(OBJ_BENCH)
	$@

build/a72_bench1024: $(OBJ) $(OBJ_BENCH) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DBENCH_CYCLES=1 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) $(OBJ_BENCH) 
	$@

build/a72_test_falcon512: $(OBJ) $(OBJ_TEST_FALCON) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9 -DAPPLE_M1=0 -o $@ $(OBJ) $(OBJ_TEST_FALCON)
	$@

build/a72_test_falcon1024: $(OBJ) $(OBJ_TEST_FALCON) $(HEAD)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=0 -o $@ $(OBJ) $(OBJ_TEST_FALCON)
	$@

build/a72_speed512_ghz: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED_Ghz)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) $(OBJ_SPEED_Ghz)
	$@

build/a72_speed1024_ghz: $(HEAD1) $(HEAD) $(OBJ) $(OBJ_SPEED_Ghz)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) $(OBJ_SPEED_Ghz)
	$@

build/a72_speed_59b_512: $(HEAD1) $(HEAD) $(OBJ)
	$(CC) $(CFLAGS) -DFALCON_LOGN=9  -DAPPLE_M1=0 -o $@ hal.c $(OBJ) speed_59b_512.c
	$@

build/a72_speed_59b_1024: $(HEAD1) $(HEAD) $(OBJ)
	$(CC) $(CFLAGS) -DFALCON_LOGN=10 -DAPPLE_M1=0 -o $@ hal.c $(OBJ) speed_59b_1024.c
	$@
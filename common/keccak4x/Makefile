KeccakP-1600-times4-SIMD256.o: KeccakP-1600-times4-SIMD256.c \
  align.h brg_endian.h KeccakP-1600-times4-SnP.h \
  KeccakP-1600-unrolling.macros SIMD256-config.h
	$(CC) -O3 -mavx2 -c $< -o $@

.PHONY: clean
clean:
	$(RM) KeccakP-1600-times4-SIMD256.o

cmake_minimum_required(VERSION 3.0)

if(MSVC)
    add_compile_options(/Wall /Wx)
else(MSVC)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c99)
endif(MSVC)

include_directories(../../../common)

add_library(kyber768 SHARED cbd.c indcpa.c kem.c kex.c ntt.c poly.c polyvec.c precomp.c reduce.c verify.c ../../../common/fips202.c ../../../common/notrandombytes.c)

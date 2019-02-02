cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror -std=c99")
include_directories(../../../common)
add_library(kyber768 SHARED cbd.c indcpa.c kem.c kex.c ntt.c poly.c polyvec.c precomp.c reduce.c verify.c ../../../common/fips202.c ../../../common/notrandombytes.c)

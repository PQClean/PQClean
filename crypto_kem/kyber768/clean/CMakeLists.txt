cmake_minimum_required(VERSION 3.0)

if(MSVC)
    add_compile_options(/Wall /Wx)
    # /Wall causes lots of errors, weaker options are /W1 or /W2 or /W3 or /W4
else(MSVC)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c99)
endif(MSVC)

include_directories(../../../common)

add_library(kyber768_clean STATIC cbd.c indcpa.c kem.c kex.c ntt.c poly.c polyvec.c precomp.c reduce.c verify.c)

LIB=libkyber768.so
OBJECTS=cbd.o indcpa.o kem.o kex.o ntt.o poly.o polyvec.o precomp.o reduce.o verify.o

COMMON_DIR=../../../common
COMMON_OBJECTS=$(COMMON_DIR)/fips202.o $(COMMON_DIR)/notrandombytes.o

CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99 -fPIC -g -I$(COMMON_DIR) $(EXTRAFLAGS)

$(LIB): $(OBJECTS) $(COMMON_OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(COMMON_OBJECTS)

clean:
	$(RM) $(OBJECTS) $(COMMON_OBJECTS)
	$(RM) $(LIB)

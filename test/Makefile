# This Makefile can be used with GNU Make or BSD Make

# override as desired
TYPE=kem
SCHEME=kyber768
IMPLEMENTATION=clean

SCHEME_DIR="../crypto_$(TYPE)/$(SCHEME)/$(IMPLEMENTATION)"
SCHEME_UPPERCASE=$(shell echo $(SCHEME) | tr a-z A-Z | sed 's/-//')

COMMON_DIR=../common
COMMON_FILES=$(COMMON_DIR)/fips202.c $(COMMON_DIR)/sha2.c
DEST_DIR=../bin

CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99 -I$(COMMON_DIR) $(EXTRAFLAGS)

all: $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION) $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION): crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/randombytes.c
	mkdir -p $(DEST_DIR)
	cd $(SCHEME_DIR) && make clean && make
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE) -I$(SCHEME_DIR) $^ -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION): crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c
	mkdir -p $(DEST_DIR)
	cd $(SCHEME_DIR) && make clean && make
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE) -I$(SCHEME_DIR) $^ -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

clean:
	$(RM) $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)

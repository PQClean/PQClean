# This Makefile can be used with GNU Make or BSD Make

# override as desired
TYPE=kem
SCHEME=kyber768
IMPLEMENTATION=clean

SCHEME_DIR="../crypto_$(TYPE)/$(SCHEME)/$(IMPLEMENTATION)"
SCHEME_UPPERCASE=$(shell echo $(SCHEME) | tr a-z A-Z | sed 's/-//')
IMPLEMENTATION_UPPERCASE=$(shell echo $(IMPLEMENTATION) | tr a-z A-Z | sed 's/-//')

COMMON_DIR=../common
COMMON_FILES=$(COMMON_DIR)/fips202.c $(COMMON_DIR)/sha2.c
COMMON_HEADERS=$(COMMON_DIR)/fips202.h $(COMMON_DIR)/randombytes.h $(COMMON_DIR)/sha2.h
DEST_DIR=../bin

# This -Wall was supported by the European Commission through the ERC Starting Grant 805031 (EPOQUE)
CFLAGS=-Wall -Wextra -Wpedantic -Werror -Wundef -std=c99 -I$(COMMON_DIR) $(EXTRAFLAGS)

all: $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION) $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)

.PHONY: build-scheme
build-scheme:
	cd $(SCHEME_DIR) && $(MAKE)

.PHONY: clean-scheme
clean-scheme:
	cd $(SCHEME_DIR) && $(MAKE) clean

.PHONY: functest
functest: $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION)

.PHONY: testvectors
testvectors: $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/test_fips202: common/fips202.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@

$(DEST_DIR)/test_sha2: common/sha2.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@

$(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION): build-scheme crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/randombytes.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION): build-scheme crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

.PHONY: clean
clean:
	$(RM) $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/test_fips202
	$(RM) $(DEST_DIR)/test_sha2

# This Makefile can be used with GNU Make or BSD Make

# override as desired
TYPE=kem
SCHEME=kyber768
IMPLEMENTATION=clean

SCHEME_DIR="../crypto_$(TYPE)/$(SCHEME)/$(IMPLEMENTATION)"
SCHEME_UPPERCASE=$(shell echo $(SCHEME) | tr a-z A-Z | sed 's/-//g')
IMPLEMENTATION_UPPERCASE=$(shell echo $(IMPLEMENTATION) | tr a-z A-Z | sed 's/-//g')

COMMON_DIR=../common
COMMON_FILES=$(COMMON_DIR)/aes.c $(COMMON_DIR)/sha2.c $(COMMON_DIR)/fips202.c $(COMMON_DIR)/sp800-185.c
COMMON_HEADERS=$(COMMON_DIR)/*.h
DEST_DIR=../bin

# This -Wall was supported by the European Commission through the ERC Starting Grant 805031 (EPOQUE)
CFLAGS=-O3 -Wall -Wextra -Wpedantic -Wvla -Werror -std=c99 \
	   -Wundef -Wshadow -Wcast-align -Wpointer-arith -Wmissing-prototypes\
	   -fstrict-aliasing -fno-common -pipe \
	   -I$(COMMON_DIR) $(EXTRAFLAGS)

# Number of tests run for functests
NTESTS=1

all: $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION) \
	$(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION) \
	$(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION)

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

.PHONY: printparams
printparams: $(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION)

.PHONY: nistkat
nistkat: $(DEST_DIR)/nistkat_$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/test_common_aes: common/aes.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@
$(DEST_DIR)/test_common_fips202: common/fips202.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@
$(DEST_DIR)/test_common_sha2: common/sha2.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@
$(DEST_DIR)/test_common_sp800-185: common/sp800-185.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@

$(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION): build-scheme crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/randombytes.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DNTESTS=$(NTESTS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION): build-scheme crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(COMMON_DIR)/notrandombytes.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION): build-scheme crypto_$(TYPE)/printparams.c
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/printparams.c -o $@

$(DEST_DIR)/nistkat_$(SCHEME)_$(IMPLEMENTATION): build-scheme crypto_$(TYPE)/nistkat.c $(COMMON_FILES) $(COMMON_DIR)/nistkatrng.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/nistkat.c $(COMMON_FILES) $(COMMON_DIR)/nistkatrng.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

.PHONY: clean
clean:
	$(RM) $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/nistkat_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/test_aes
	$(RM) $(DEST_DIR)/test_fips202
	$(RM) $(DEST_DIR)/test_sha2
	$(RM) $(DEST_DIR)/test_sp800-185

.PHONY: distclean
distclean:
	$(RM) -r $(DEST_DIR)

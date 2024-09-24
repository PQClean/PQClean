# This Makefile can be used with GNU Make or BSD Make

# override as desired
TYPE=kem
SCHEME=ml-kem-768
IMPLEMENTATION=clean
KAT_RNG=nist

SCHEME_DIR=../crypto_$(TYPE)/$(SCHEME)/$(IMPLEMENTATION)
SCHEME_UPPERCASE=$(shell echo $(SCHEME) | tr a-z A-Z | sed 's/-//g')
IMPLEMENTATION_UPPERCASE=$(shell echo $(IMPLEMENTATION) | tr a-z A-Z | sed 's/-//g')

TEST_COMMON_DIR=../test/common
COMMON_DIR=../common
COMMON_FILES=$(COMMON_DIR)/aes.c $(COMMON_DIR)/sha2.c $(COMMON_DIR)/fips202.c $(COMMON_DIR)/nistseedexpander.c $(COMMON_DIR)/sp800-185.c
COMMON_HEADERS=$(COMMON_DIR)/*.h
DEST_DIR=../bin

SCHEME_LIBRARY=$(SCHEME_DIR)/lib$(SCHEME)_$(IMPLEMENTATION).a
SCHEME_FILES=$(wildcard $(SCHEME_DIR)/*.[chsS])

ifeq ($(SCHEME), falcon-512)
	INTEROP=falcon-padded-512
else ifeq ($(SCHEME), falcon-1024)
	INTEROP=falcon-padded-1024
else ifeq ($(SCHEME), falcon-padded-512)
	INTEROP=falcon-512
else ifeq ($(SCHEME), falcon-padded-1024)
	INTEROP=falcon-1024
endif

ifdef INTEROP
	INTEROP_UPPERCASE=$(shell echo $(INTEROP) | tr a-z A-Z | sed 's/-//g')
	INTEROP_DIR=../crypto_$(TYPE)/$(INTEROP)/$(IMPLEMENTATION)
	INTEROP_LIBRARY=$(INTEROP_DIR)/lib$(INTEROP)_$(IMPLEMENTATION).a
	override EXTRAFLAGS+=-DPQCLEAN_FALCON_TEST_INTEROP \
			   -DPQCLEAN_INTEROP_NAMESPACE=PQCLEAN_$(INTEROP_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) \
			   -DPQCLEAN_INTEROP_INCLUDE=$(INTEROP_DIR)
	EXTRALINKERFLAGS=-L$(INTEROP_DIR) -l$(INTEROP)_$(IMPLEMENTATION)
endif

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
build-scheme: $(SCHEME_LIBRARY)

$(SCHEME_LIBRARY): $(SCHEME_FILES)
	cd $(SCHEME_DIR) && $(MAKE)

.PHONY: clean-scheme
clean-scheme:
	cd $(SCHEME_DIR) && $(MAKE) clean

ifdef INTEROP
.PHONY: build-interop
build-interop: $(INTEROP_LIBRARY)

$(INTEROP_LIBRARY): $(INTEROP_FILES)
	cd $(INTEROP_DIR) && $(MAKE)

.PHONY: clean-interop
clean-interop:
	cd $(INTEROP_DIR) && $(MAKE) clean
endif

.PHONY: functest
functest: $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION)

.PHONY: testvectors
testvectors: $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)

.PHONY: printparams
printparams: $(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION)

.PHONY: nistkat
nistkat: $(DEST_DIR)/nistkat_$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/test_common_aes: test_common/aes.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@
$(DEST_DIR)/test_common_fips202: test_common/fips202.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@
$(DEST_DIR)/test_common_sha2: test_common/sha2.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@
$(DEST_DIR)/test_common_sp800-185: test_common/sp800-185.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@

$(DEST_DIR)/test_common_nistseedexpander: test_common/sp800-185.c $(COMMON_FILES)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) $< $(COMMON_FILES) -o $@

$(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION): $(SCHEME_LIBRARY) $(INTEROP_LIBRARY) crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/randombytes.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DNTESTS=$(NTESTS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/functest.c $(COMMON_FILES) $(COMMON_DIR)/randombytes.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION) $(EXTRALINKERFLAGS)

$(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION): $(SCHEME_LIBRARY) crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(TEST_COMMON_DIR)/notrandombytes.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/testvectors.c $(COMMON_FILES) $(TEST_COMMON_DIR)/notrandombytes.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

$(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION): crypto_$(TYPE)/printparams.c
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/printparams.c -o $@

$(DEST_DIR)/nistkat_$(SCHEME)_$(IMPLEMENTATION): $(SCHEME_LIBRARY) crypto_$(TYPE)/$(KAT_RNG)kat.c $(COMMON_FILES) $(TEST_COMMON_DIR)/$(KAT_RNG)katrng.c $(COMMON_HEADERS)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -DPQCLEAN_NAMESPACE=PQCLEAN_$(SCHEME_UPPERCASE)_$(IMPLEMENTATION_UPPERCASE) -I$(SCHEME_DIR) crypto_$(TYPE)/$(KAT_RNG)kat.c $(COMMON_FILES) $(TEST_COMMON_DIR)/$(KAT_RNG)katrng.c -o $@ -L$(SCHEME_DIR) -l$(SCHEME)_$(IMPLEMENTATION)

.PHONY: clean
clean:
	$(RM) $(DEST_DIR)/functest_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/testvectors_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/printparams_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/nistkat_$(SCHEME)_$(IMPLEMENTATION)
	$(RM) $(DEST_DIR)/test_common_aes
	$(RM) $(DEST_DIR)/test_common_fips202
	$(RM) $(DEST_DIR)/test_common_sha2
	$(RM) $(DEST_DIR)/test_common_sp800-185
	$(RM) $(DEST_DIR)/test_common_nistseedexpander

.PHONY: distclean
distclean:
	$(RM) -r $(DEST_DIR)

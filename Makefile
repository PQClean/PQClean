
# assumes a SCHEME variable; e.g. make functest_kem SCHEME=crypto_kem/kyber768
ifndef SCHEME
	# TODO make this more granular, i.e. make clean should not require SCHEME
$(error SCHEME variable is not set)
endif

CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99

functest: $(dir $(SCHEME))test.c $(wildcard $(SCHEME)/clean/*.c) $(wildcard $(SCHEME)/clean/*.h)
	mkdir -p bin
	$(CC) $(CFLAGS)\
		-I"./common/"\
		-I"$(SCHEME)/clean/"\
		-o bin/functest_$(subst /,_,$(SCHEME))\
		common/*.c\
		$(SCHEME)/clean/*.c\
		$<

.PHONY: clean

clean:
	rm -rf bin

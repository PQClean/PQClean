# This -Wall was supported by the European Commission through the ERC Starting Grant 805031 (EPOQUE)
CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c99 $(EXTRAFLAGS)

ALL_SCHEMES=$(filter-out crypto_%/test.c, $(wildcard crypto_*/*))

default: help

.PHONY: require_scheme
require_scheme:
# assumes a SCHEME variable; e.g. make functest_kem SCHEME=crypto_kem/kyber768
ifndef SCHEME
	$(error The SCHEME variable is not set. Example: SCHEME=crypto_kem/kyber768)
endif

bin/functest_$(subst /,_,$(SCHEME)): $(dir $(SCHEME))test.c $(wildcard $(SCHEME)/clean/*.c) $(wildcard $(SCHEME)/clean/*.h) | require_scheme
	mkdir -p bin
	$(CC) $(CFLAGS) \
		-iquote "./common/" \
		-iquote "$(SCHEME)/clean/" \
		-o bin/functest_$(subst /,_,$(SCHEME)) \
		common/*.c \
		$(SCHEME)/clean/*.c \
		$<

.PHONY: functest
functest: bin/functest_$(subst /,_,$(SCHEME))

.PHONY: run-functest
run-functest: bin/functest_$(subst /,_,$(SCHEME))
	./$<

.PHONY: clean
clean:
	rm -rf bin

.PHONY: format
format:
	find . -iname *.h -o -iname *.c | xargs clang-format -i -style=file

.PHONY: check-format
check-format:
	@for src in $(SOURCES) ; do \
		var=`clang-format "$(SRC_DIR)/$$src" | diff "$(SRC_DIR)/$$src" - | wc -l` ; \
		if [ $$var -ne 0 ] ; then \
			echo "$$src does not respect the coding style (diff: $$var lines)" ; \
			exit 1 ; \
		fi ; \
	done
	@echo "Style check passed"

.PHONY: tidy
tidy:
	$(eval undefine .TIDY_FIX)
	$(MAKE) do-tidy

do-tidy: require_scheme
	clang-tidy \
		-quiet $(.TIDY_FIX) \
		$(SCHEME)/clean/*.c \
		$(SCHEME)/../test.c \
		common/*.c \
		-- -iquote "common/" -iquote "$(SCHEME)/clean"

.PHONY: apply-tidy
apply-tidy:
	$(MAKE) do-tidy .TIDY_FIX=-fix

# The below should be outlined with ts=8
.PHONY: help
help:
	@echo "make functest SCHEME=scheme		Build functional tests for SCHEME"
	@echo "make run-functest SCHEME=scheme		Run functional tests for SCHEME"
	@echo "make run-functest-all			Run all functests"
	@echo "make clean				Clean up the bin/ folder"
	@echo "make format				Automatically formats all the source code"
	@echo "make tidy SCHEME=scheme  		Runs the clang-tidy linter against SCHEME"
	@echo "make apply-tidy SCHEME=scheme		Tries to automatically fix the issues found by clang-tidy in SCHEME"
	@echo "make tidy-all				Runs the clang-tidy linter against all schemes"
	@echo "make apply-tidy-all			Tidy up all schemes"
	@echo "make help				Displays this message"

.PHONY: functest-all
build-functests: $(foreach skeme,$(ALL_SCHEMES),bin/functest_$(subst /,_,$(skeme)))

.PHONY: run-functest-all
run-functest-all: build-functests
	@for functest in bin/functest_* ; do \
		echo ./$$functest ; \
		./$$functest ; \
	done
	@echo Tests completed

.PHONY: test-all
test-all: run-functests

.PHONY: tidy-all
tidy-all:
	@for scheme in $(ALL_SCHEMES); do \
		$(MAKE) tidy SCHEME=$$scheme; \
	done

.PHONY: apply-tidy-all
apply-tidy-all:
	@for scheme in $(ALL_SCHEMES); do \
		$(MAKE) apply-tidy SCHEME=$$scheme; \
	done

ALL_SCHEMES=$(filter-out crypto_%.c, $(wildcard crypto_*/*))

default: help

.PHONY: require_scheme
require_scheme:
# assumes a SCHEME variable; e.g. make functest SCHEME=crypto_kem/kyber768
ifndef SCHEME
	$(error The SCHEME variable is not set. Example: SCHEME=crypto_kem/kyber768)
endif

.PHONY: clean
clean:
	rm -rf bin

.PHONY: format
format:
	astyle --project crypto_*/*/*/*.[ch] common/*.[ch]

.PHONY: check-format
check-format:
	astyle --dry-run --project crypto_*/*/*/*.[ch] common/*.[ch] | grep Formatted && exit 1 || exit 0

.PHONY: tidy
tidy:
	$(MAKE) do-tidy

do-tidy: require_scheme
	clang-tidy \
		-quiet $(.TIDY_FIX) \
		$(SCHEME)/clean/*.c \
		common/*.c \
		-- -iquote "common/" -iquote "$(SCHEME)/clean"

.PHONY: apply-tidy
apply-tidy:
	$(MAKE) do-tidy .TIDY_FIX=-fix

# The below should be outlined with ts=8
.PHONY: help
help:
	@echo "make clean				Clean up the bin/ folder"
	@echo "make format				Automatically formats all the source code"
	@echo "make tidy SCHEME=scheme  		Runs the clang-tidy linter against SCHEME"
	@echo "make apply-tidy SCHEME=scheme		Tries to automatically fix the issues found by clang-tidy in SCHEME"
	@echo "make tidy-all				Runs the clang-tidy linter against all schemes"
	@echo "make apply-tidy-all			Tidy up all schemes"
	@echo "make help				Displays this message"


.PHONY: tidy-all
tidy-all:
	@for scheme in $(ALL_SCHEMES); do \
		$(MAKE) tidy SCHEME=$$scheme || exit 1 ; \
	done

.PHONY: apply-tidy-all
apply-tidy-all:
	@for scheme in $(ALL_SCHEMES); do \
		$(MAKE) apply-tidy SCHEME=$$scheme; \
	done

EMULATOR := CPUEmulator.bat
# fill/Fill.tst requires running in GUI
PROGRAMS := fill/FillAutomatic mult/Mult

all:

test: $(PROGRAMS)

$(PROGRAMS):
	@echo "Testing $@.tst"
	@OUTPUT="$(shell $(EMULATOR) $@.tst 2>&1)"; \
	if echo "$$OUTPUT" | grep -q "failure"; then \
		echo "$$OUTPUT from $@.tst" ; \
		exit 1; \
	fi

.PHONY: clean

clean:
	@echo "Cleaning projects/04"
	@rm -f fill/*.hack
	@rm -f fill/*.out
	@rm -f mult/*.hack
	@rm -f mult/*.out

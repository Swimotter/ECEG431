SIMULATOR := HardwareSimulator.bat
PROGRAMS := Off

all:

test: $(PROGRAMS)

$(PROGRAMS):
	@echo "Testing $@.tst"
	@OUTPUT="$(shell $(SIMULATOR) $@.tst 2>&1)"; \
	if echo "$$OUTPUT" | grep -q "failure"; then \
		echo "$$OUTPUT from $@.tst" ; \
		exit 1; \
	fi

.PHONY: clean

clean:
	@echo "Cleaning projects/00"
	@rm -f *.out

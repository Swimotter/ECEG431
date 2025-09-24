SIMULATOR := HardwareSimulator.bat
PROGRAMS := And And16 DMux DMux4Way DMux8Way Mux Mux4Way16 Mux8Way16 Mux16 Not Not16 Or Or8Way Or16 Xor

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
	@echo "Cleaning projects/01"
	@rm -f *.out

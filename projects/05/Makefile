SIMULATOR := HardwareSimulator.bat
# Memory.tst requires running in GUI
PROGRAMS := ComputerAdd ComputerMax ComputerRect CPU-external CPU

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
	@echo "Cleaning projects/05"
	@rm -f *.out

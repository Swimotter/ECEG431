SIMULATOR := HardwareSimulator.bat
PROGRAMS := Add16 ALU-basic ALU FullAdder HalfAdder Inc16

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
	@echo "Cleaning projects/02"
	@rm -f *.out

SIMULATOR := HardwareSimulator.bat
PROGRAMS := a/Bit a/PC a/RAM8 a/RAM64 a/Register b/RAM4K b/RAM16K b/RAM512

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
	@echo "Cleaning projects/03"
	@rm -f a/*.out
	@rm -f b/*.out

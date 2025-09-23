SIMULATOR := HardwareSimulator.bat
PROGRAMS := Off

all:

test: $(PROGRAMS)

$(PROGRAMS):
	$(SIMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f *.out

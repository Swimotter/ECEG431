SIMULATOR := HardwareSimulator.bat
# Memory.tst requires running in GUI
PROGRAMS := ComputerAdd ComputerMax ComputerRect CPU-external CPU

all:

test: $(PROGRAMS)

$(PROGRAMS):
	$(SIMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f *.out

SIMULATOR := HardwareSimulator.bat
PROGRAMS := And And16 DMux DMux4Way DMux8Way Mux Mux4Way16 Mux8Way16 Mux16 Not Not16 Or Or8Way Or16 Xor

all:

test: $(PROGRAMS)

$(PROGRAMS):
	$(SIMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f *.out

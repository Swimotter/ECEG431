SIMULATOR := HardwareSimulator.bat
PROGRAMS := Add16 ALU-basic ALU FullAdder HalfAdder Inc16

all:

test: $(PROGRAMS)

$(PROGRAMS):
	$(SIMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f *.out

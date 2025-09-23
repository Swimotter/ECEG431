SIMULATOR := HardwareSimulator.bat
PROGRAMS := Off

all: $(PROGRAMS)

$(PROGRAMS):
	$(SIMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f *.out

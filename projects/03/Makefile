SIMULATOR := HardwareSimulator.bat
PROGRAMS := a/Bit a/PC a/RAM8 a/RAM64 a/Register b/RAM4K b/RAM16K b/RAM512

all:

test: $(PROGRAMS)

$(PROGRAMS):
	$(SIMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f a/*.out
	rm -f b/*.out

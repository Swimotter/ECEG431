EMULATOR := CPUEmulator.bat
# fill/Fill.tst requires running in GUI
PROGRAMS := fill/FillAutomatic mult/Mult

all: $(PROGRAMS)

$(PROGRAMS):
	$(EMULATOR) $@.tst

.PHONY: clean

clean:
	rm -f fill/*.hack
	rm -f fill/*.out
	rm -f mult/*.hack
	rm -f mult/*.out

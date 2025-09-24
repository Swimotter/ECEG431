# LOLCODE doesn't have a working windows port
SUBDIRS := C++

all:
	@echo "Building projects/07"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir all; \
	done

test:
	@echo "Testing projects/07"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir test; \
	done

.PHONY: clean

clean:
	@echo "Cleaning projects/07"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir clean; \
	done

	@rm -f ../StackArithmetic/SimpleAdd/*.out
	@rm -f ../StackArithmetic/SimpleAdd/*.asm
	@rm -f ../StackArithmetic/StackTest/*.out
	@rm -f ../StackArithmetic/StackTest/*.asm
	@rm -f ../MemoryAccess/BasicTest/*.out
	@rm -f ../MemoryAccess/BasicTest/*.asm
	@rm -f ../MemoryAccess/PointerTest/*.out
	@rm -f ../MemoryAccess/PointerTest/*.asm
	@rm -f ../MemoryAccess/StaticTest/*.out
	@rm -f ../MemoryAccess/StaticTest/*.asm
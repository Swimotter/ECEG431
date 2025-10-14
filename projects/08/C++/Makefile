CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2
SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../ProgramFlow/BasicLoop ../ProgramFlow/FibonacciSeries ../FunctionCalls/SimpleFunction ../FunctionCalls/FibonacciElement ../FunctionCalls/StaticsTest
VM_TRANSLATOR := bin/vm_translator.exe
EMULATOR := CPUEmulator.bat
SUBMIT_FILE := exe.txt

all: builddir $(VM_TRANSLATOR)
	@echo $(VM_TRANSLATOR) > $(SUBMIT_FILE)

test: translate
	@for dir in $(PROGRAMS); do \
		echo "Testing $$dir"; \
		OUTPUT="$$( $(EMULATOR) "$$dir/$$(basename $$dir).tst" 2>&1 )"; \
		echo "$$OUTPUT" | grep -q "success" || { echo "$$OUTPUT"; exit 1; }; \
	done

translate: builddir $(PROGRAMS)

.FORCE:

$(PROGRAMS): $(VM_TRANSLATOR) .FORCE
	@$(VM_TRANSLATOR) $@


$(VM_TRANSLATOR): $(OBJS)
	@echo "Linking $(notdir $@)"
	@$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	@echo "Compiling $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $^ -o $@

.PHONY: clean clean-test builddir

clean: clean-test
	@echo "Cleaning projects/08/C++"
	@rm -rf bin/

clean-test:
	@echo "Cleaning tests for projects/08"
	@$(foreach dir, $(PROGRAMS), \
		@rm -f $(dir)/*.out \
		@rm -f $(dir)/*.asm \
	)

builddir:
	@mkdir -p $(dir $(VM_TRANSLATOR))

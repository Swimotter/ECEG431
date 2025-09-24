CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2

SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../StackArithmetic/SimpleAdd/SimpleAdd ../StackArithmetic/StackTest/StackTest ../MemoryAccess/BasicTest/BasicTest ../MemoryAccess/PointerTest/PointerTest ../MemoryAccess/StaticTest/StaticTest
VM_TRANSLATOR := bin/vm_translator.exe
EMULATOR := CPUEmulator.bat

all: builddir $(VM_TRANSLATOR)

test: $(PROGRAMS)

$(PROGRAMS): $(VM_TRANSLATOR)
	$(VM_TRANSLATOR) $@.vm
	$(EMULATOR) $@.tst

$(VM_TRANSLATOR): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $^ -o $@

.PHONY: clean builddir

clean:
	rm -rf bin/
	rm -f ../StackArithmetic/SimpleAdd/*.out
	rm -f ../StackArithmetic/StackTest/*.out
	rm -f ../MemoryAccess/BasicTest/*.out
	rm -f ../MemoryAccess/PointerTest/*.out
	rm -f ../MemoryAccess/StaticTest/*.out

builddir:
	mkdir -p $(dir $(VM_TRANSLATOR))
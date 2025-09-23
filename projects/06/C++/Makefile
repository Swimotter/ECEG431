CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2

SRC := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

PROGRAMS := ../add/Add ../max/Max ../max/MaxL ../pong/Pong ../pong/PongL ../rect/Rect ../rect/RectL
ASSEMBLER := bin/assembler.exe
BUILT_IN_ASSEMBLER := Assembler.bat

all: $(ASSEMBLER)

test: $(PROGRAMS)

$(PROGRAMS): $(ASSEMBLER)
	$(ASSEMBLER) $@.asm
	mv $@.hack $@-C++.hack
	sleep 1s
	$(BUILT_IN_ASSEMBLER) $@.asm
	diff $@-C++.hack $@.hack

$(ASSEMBLER): $(SRC)
	mkdir -p $(dir $(ASSEMBLER))
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) $^ -o $@

.PHONY: clean

clean:
	rm -rf bin/
	rm -f ../add/*.hack
	rm -f ../max/*.hack
	rm -f ../pong/*.hack
	rm -f ../rect/*.hack

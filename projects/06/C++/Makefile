CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2

SRC := $(wildcard src/*.cpp)

PROGRAMS := ../add/Add ../max/Max ../max/MaxL ../pong/Pong ../pong/PongL ../rect/Rect ../rect/RectL
ASSEMBLER := bin/assembler.exe

all: $(PROGRAMS)

$(PROGRAMS): $(ASSEMBLER)
	$(ASSEMBLER) $@.asm
	mv $@.hack $@-C++.hack

$(ASSEMBLER): $(SRC)
	mkdir $(dir $(ASSEMBLER))
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -rf bin/
	rm -f ../add/*-C++.hack
	rm -f ../max/*-C++.hack
	rm -f ../pong/*-C++.hack
	rm -f ../rect/*-C++.hack

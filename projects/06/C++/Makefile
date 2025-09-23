CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2

SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../add/Add ../max/Max ../max/MaxL ../pong/Pong ../pong/PongL ../rect/Rect ../rect/RectL
ASSEMBLER := bin/assembler.exe
BUILT_IN_ASSEMBLER := Assembler.bat

all: builddir $(ASSEMBLER)

test: $(PROGRAMS)

$(PROGRAMS): $(ASSEMBLER)
	$(ASSEMBLER) $@.asm
	mv $@.hack $@-C++.hack
	sleep 1s
	$(BUILT_IN_ASSEMBLER) $@.asm
	diff $@-C++.hack $@.hack

$(ASSEMBLER): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $^ -o $@

.PHONY: clean builddir

clean:
	rm -rf bin/
	rm -f ../add/*.hack
	rm -f ../max/*.hack
	rm -f ../pong/*.hack
	rm -f ../rect/*.hack

builddir:
	mkdir -p $(dir $(ASSEMBLER))
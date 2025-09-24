CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2

SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../add/Add ../max/Max ../max/MaxL ../pong/Pong ../pong/PongL ../rect/Rect ../rect/RectL
ASSEMBLER := bin/assembler.exe
BUILT_IN_ASSEMBLER := Assembler.bat

all: builddir $(ASSEMBLER)

test: builddir $(PROGRAMS)

$(PROGRAMS): $(ASSEMBLER)
	@echo "Testing $@.tst"
	@$(ASSEMBLER) $@.asm
	@mv $@.hack $@-C++.hack
	@sleep 1s
	@$(BUILT_IN_ASSEMBLER) $@.asm
	@diff $@-C++.hack $@.hack

$(ASSEMBLER): $(OBJS)
	@echo "Linking $(notdir $@)"
	@$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	@echo "Compiling $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $^ -o $@

.PHONY: clean builddir

clean:
	@echo "Cleaning projects/06/C++"
	@rm -rf bin/
	@rm -f ../add/*.hack
	@rm -f ../max/*.hack
	@rm -f ../pong/*.hack
	@rm -f ../rect/*.hack

builddir:
	@echo "Testing projects/06/C++"
	@mkdir -p $(dir $(ASSEMBLER))
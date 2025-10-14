CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2
SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../HelloWorld ../Average ../ConvertToBin ../ComplexArrays ../Fraction ../List ../Square ../Pong
JACK_COMPILER := bin/jack_compiler.exe
EMULATOR := JackCompiler.bat
SUBMIT_FILE := exe.txt

all: builddir $(JACK_COMPILER)
	@echo $(JACK_COMPILER) > $(SUBMIT_FILE)

test: translate
	@for dir in $(PROGRAMS); do \
		@$(EMULATOR) $(dir) \
		@FILES = $(basename $(wildcard $(dir)/*.vm))
		@$(foreach file, $(FILES), \
			@diff $(file)-C++.vm $(file).vm \
		) \
	done

translate: builddir $(PROGRAMS)

.FORCE:

$(PROGRAMS): $(JACK_COMPILER) .FORCE
	@$(JACK_COMPILER) $@
	@FILES = $(basename $(wildcard $@/*.vm))
	@$(foreach dir, $(FILES), \
		@mv $(dir).vm $(dir)-C++.vm \
	)

$(JACK_COMPILER): $(OBJS)
	@echo "Linking $(notdir $@)"
	@$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	@echo "Compiling $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $^ -o $@

.PHONY: clean clean-test builddir

clean: clean-test
	@echo "Cleaning projects/09/C++"
	@rm -rf bin/

clean-test:
	@echo "Cleaning tests for projects/08"
	@$(foreach dir, $(PROGRAMS), \
		@rm -f $(dir)/*.vm \
	)

builddir:
	@mkdir -p $(dir $(JACK_COMPILER))

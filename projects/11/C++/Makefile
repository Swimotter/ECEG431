CXX := g++
CXXFLAGS := -std=c++20 -Wall -O0 -g
SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../ComplexArrays ../ConvertToBin ../Pong ../Seven ../Square
JACK_COMPILER := bin/jack_compiler.exe
COMPILER := JackCompiler.bat
SUBMIT_FILE := exe.txt

all: builddir $(JACK_COMPILER)
	@echo $(JACK_COMPILER) > $(SUBMIT_FILE)

test: translate
	@for dir in $(PROGRAMS); do \
		for file in $$dir/*.jack; do \
			echo "Testing $$file"; \
			mv $${file%%.jack}.vm $${file%%.jack}-C++.vm; \
			$(COMPILER) $$file; \
			diff $${file%.jack}-C++.vm $${file%.jack}.vm; \
		done; \
	done

translate: builddir $(PROGRAMS)

.FORCE:

$(PROGRAMS): $(JACK_COMPILER) .FORCE
	@$(JACK_COMPILER) $@

$(JACK_COMPILER): $(OBJS)
	@echo "Linking $(notdir $@)"
	@$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	@echo "Compiling $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $^ -o $@

.PHONY: clean clean-test builddir

clean: clean-test
	@echo "Cleaning projects/11/C++"
	@rm -f $(SUBMIT_FILE)
	@rm -rf bin/

clean-test:
	@echo "Cleaning tests for projects/11"
	@$(foreach dir, $(PROGRAMS), \
		@rm -f $(dir)/*.vm \
	)

builddir:
	@mkdir -p $(dir $(JACK_COMPILER))

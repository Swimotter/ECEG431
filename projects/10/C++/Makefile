CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2
SRCS := $(wildcard src/*.cpp)
INCLUDE_FOLDER := include

OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRCS))

PROGRAMS := ../ArrayTest ../ExpressionLessSquare ../Square
JACK_COMPILER := bin/jack_compiler.exe
SUBMIT_FILE := exe.txt

all: builddir $(JACK_COMPILER)
	@echo $(JACK_COMPILER) > $(SUBMIT_FILE)

test: translate
	@for dir in $(PROGRAMS); do \
		for file in $$dir/*.jack; do \
			echo "Testing $$file"; \
			diff $${file%.jack}-C++.xml $${file%.jack}.xml; \
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
	@echo "Cleaning projects/10/C++"
	@rm -f $(SUBMIT_FILE)
	@rm -rf bin/

clean-test:
	@echo "Cleaning tests for projects/10"
	@$(foreach dir, $(PROGRAMS), \
		@rm -f $(dir)/*-C++.xml \
	)

builddir:
	@mkdir -p $(dir $(JACK_COMPILER))

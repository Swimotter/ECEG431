SUBDIRS := projects/00 projects/01 projects/02 projects/03 projects/04 projects/05 projects/06 projects/07

# Generates files without testing
all:
	@echo "Building projects"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir all; \
	done

# Generates files and runs tests
test:
	@echo "Testing projects"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir test; \
	done

.PHONY: clean

clean:
	@echo "Cleaning projects"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir clean; \
	done
SUBDIRS := projects/00 projects/01 projects/02 projects/03 projects/04 projects/05 projects/06

# Generates files without testing
all:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir all; \
	done

# Generates files and runs tests
test:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir test; \
	done

.PHONY: clean

clean:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir clean; \
	done
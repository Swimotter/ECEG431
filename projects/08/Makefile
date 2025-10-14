SUBDIRS := C++

all:
	@echo "Building projects/08"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir all; \
	done

test:
	@echo "Testing projects/08"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir test; \
	done

.PHONY: clean

clean:
	@echo "Cleaning projects/08"
	@for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir clean; \
	done
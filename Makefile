SUBDIRS = projects/00 projects/01 projects/02 projects/03 projects/04 projects/05 projects/06

all:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir all; \
	done

.PHONY: clean

clean:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir clean; \
	done
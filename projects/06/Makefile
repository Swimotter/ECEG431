# LOLCODE doesn't have a working windows port
SUBDIRS = C++

all:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir all; \
	done

test:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir test; \
	done

.PHONY: clean

clean:
	for dir in $(SUBDIRS); do \
		"$(MAKE)" -C $$dir clean; \
	done

	rm -f ../add/*.hack
	rm -f ../max/*.hack
	rm -f ../pong/*.hack
	rm -f ../rect/*.hack
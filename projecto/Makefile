CC=gcc
CFLAGS=-Wall -std=c99 -g
INCDIR=include
SRCDIR=src
OBJECTS=options.o

all: ddt

%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) -c -o $@ $< $(CFLAGS) -I$(INCDIR)

ddt: $(SRCDIR)/main.c $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) -I$(INCDIR)

.PHONY: docs
docs:
	doxygen Doxyfile

.PHONY: clean
clean:
	touch dummy.o
	rm *.o
	touch doc/dummy
	rm -r doc/*
	
.PHONY: rm
rm:
	touch ddt
	touch dummy.o
	rm *.o
	rm ddt
	touch doc/dummy
	rm -r doc/*

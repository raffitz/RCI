CC=gcc
CFLAGS=-Wall -std=c99
INCDIR=include
SRCDIR=src
OBJECTS=

all: ddt

%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $< $(CFLAGS) -I$(INCDIR)

ddt: $(SRCDIR)/main.c $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) -I$(INCDIR)
.PHONY: docs
docs:
	doxygen Doxyfile

.PHONY: clean
clean:
	rm ddt *.o

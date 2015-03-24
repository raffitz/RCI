CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g -DRCIDEBUG1
INCDIR=include
SRCDIR=src
OBJECTS=options.o net_common.o net_udp.o net_tcp.o interface.o ring.o message_handler.o fd_list.o common.o

all: ddt

%.o: $(SRCDIR)/%.c $(INCDIR)/%.h $(INCDIR)/common.h
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

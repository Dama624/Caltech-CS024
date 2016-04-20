CC = gcc
CFLAGS = -g -Wall -Werror
ASFLAGS = -g

# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: testunacceptable testmyalloc simpletest


clean:
	rm -rf *.o *~ testunacceptable testmyalloc simpletest

unacceptable_myalloc.o:	unacceptable_myalloc.c myalloc.h
sequence.o:	sequence.h sequence.c
myalloc.o:	myalloc.c myalloc.h
testalloc.o:	testalloc.c myalloc.h sequence.h
simpletest.o:	simpletest.c myalloc.h

testunacceptable: testalloc.o unacceptable_myalloc.o sequence.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testmyalloc: testalloc.o myalloc.o sequence.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

simpletest: simpletest.o myalloc.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


.PHONY: all clean


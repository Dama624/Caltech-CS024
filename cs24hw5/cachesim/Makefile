# CC=gcc
CFLAGS=-O2 -Wall -Werror
#CFLAGS=-g -O0 -Wall -Werror


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: testmem heaptest apsptest qsorttest


membase.o:	membase.c membase.h
memory.o:	memory.c memory.h membase.h
cache.o:	cache.c cache.h membase.h
cmdline.o:	cmdline.c cmdline.h membase.h memory.h cache.h

testmem.o:	testmem.c membase.h memory.h cache.h

heap.o:		heap.h membase.h
heaptest.o:	heap.h membase.h memory.h cache.h

apsptest.o:	membase.h memory.h cache.h

qsorttest.o:	membase.h memory.h cache.h

testmem: membase.o memory.o cache.o testmem.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

heaptest: membase.o memory.o cache.o cmdline.o heap.o heaptest.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

apsptest: membase.o memory.o cache.o cmdline.o apsptest.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

qsorttest: membase.o memory.o cache.o cmdline.o qsorttest.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	-rm -f *.o testmem heaptest apsptest qsorttest


.PHONY: all clean


CC = gcc
CFLAGS = -g -O0 -Wall -Werror
LDFLAGS = -lpthread


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: testcas testaccum


testcas: cmp_swap.o testcas.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

testaccum: cmp_swap.o accum.o testaccum.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f testcas testaccum *.o


.PHONY: all clean


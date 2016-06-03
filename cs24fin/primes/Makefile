# CC=gcc
CFLAGS=-O2 -DNDEBUG -Wall -Werror

# FOR DEBUGGING:
#   CFLAGS=-g -O0


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: psieve


psieve: bitvector.o psieve.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


clean:
	rm -f *~ *.o psieve psieve.exe


.PHONY: all clean

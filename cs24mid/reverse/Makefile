# CC = gcc
ASFLAGS = -g
CFLAGS = -g -O0 -Wall -Werror


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: list

reverse.o: reverse.s


list: main.o list.o reverse.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *~ list

.PHONY: all clean


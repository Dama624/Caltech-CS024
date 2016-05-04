CC = gcc
CFLAGS = -Wall -g -O0


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: testarb

testarb: arbiter.o bus.o testarb.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ *.o testarb testarb.exe

.PHONY:  clean all


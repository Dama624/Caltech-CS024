CC = gcc
CFLAGS = -g -Wall -Werror
ASFLAGS = -g

# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: rlenc rldec test_rldec


rlenc: rlenc.o
	$(CC) $(CFLAGS) rlenc.o -o rlenc $(LDFLAGS)

rldec: rldec.o rl_decode.o
	$(CC) $(CFLAGS) rldec.o rl_decode.o -o rldec $(LDFLAGS)

test_rldec: test_rldec.o rl_decode.o
	$(CC) $(CFLAGS) test_rldec.o rl_decode.o -o test_rldec $(LDFLAGS)

clean:
	rm -f *~ rlenc.o rldec.o test_rldec.o rl_decode.o \
	rlenc rlenc.exe rldec rldec.exe test_rldec test_rldec.exe

.PHONY: all clean


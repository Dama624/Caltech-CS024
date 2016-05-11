CC=gcc
ASFLAGS=-march i686
CFLAGS=-Wall -Werror


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif



OBJS=cpuid.o cpuid_ext.o cpuinfo.o

all: cpuinfo

cpuinfo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cpuinfo $(LDFLAGS)

clean:
	rm -f $(OBJS) *~ cpuinfo

.PHONY: clean


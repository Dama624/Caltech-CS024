CC=gcc

# For performance testing:
# CFLAGS = -Wall -DNDEBUG -O2

# For debugging:
CFLAGS = -Wall -g -O0 -DDEBUG_ZERO


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all:  mmtest mmperf
opt:  ommtest ommperf

mmtest: mmtest.o mm_impl.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

mmperf: mmperf.o mm_impl.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

ommtest: mmtest.o opt_mm_impl.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

ommperf: mmperf.o opt_mm_impl.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f mmtest mmperf ommtest ommperf *.o *~

.PHONY: all opt clean


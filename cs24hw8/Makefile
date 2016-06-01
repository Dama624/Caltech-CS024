CC = gcc
CFLAGS = -Wall -Werror -g -O0

VMEM_OBJS = virtualmem.o vmalloc.o matrix.o test_matrix.o

# So that the binary programs can be listed in fewer places.
# You will want to add to this variable as you implement various policies.
BINARIES = test_matrix test_matrix_fifo test_matrix_age


# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all: $(BINARIES)

# Compile this file with optimizations so that it accesses memory in
# reasonable ways.
matrix.o: CFLAGS += -O2

test_matrix: $(VMEM_OBJS) vmpolicy_random.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_matrix_fifo: $(VMEM_OBJS) vmpolicy_fifo.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_matrix_age: $(VMEM_OBJS) vmpolicy_age.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o *~ $(BINARIES)


.PHONY: all clean


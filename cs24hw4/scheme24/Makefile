OBJS=ptr_vector.o values.o alloc.o parse.o special_forms.o \
	native_lambdas.o evaluator.o repl.o

CC = gcc

# The specified warning is disabled so that students can compile the code
# before implementing their part of it.
CFLAGS=-Wall -Werror -g -O0 -Wno-unused-but-set-variable

LDFLAGS=-lm

# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


all:  scheme24

scheme24: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o scheme24 $(LDFLAGS)

docs:
	doxygen

clean:
	rm -f *.gch *.o *~ scheme24
	rm -rf docs/html

.PHONY: all clean docs

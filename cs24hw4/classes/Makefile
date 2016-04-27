CC = gcc
CFLAGS = -g -O0 -Wall -Werror

# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


shapeinfo : shapes.o shapeinfo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# dependencies on header files
shapes.c : shapes.h
shapeinfo.c : shapes.h

clean :
	rm -f *.o shapeinfo shapeinfo.exe *~


.PHONY : clean


#
# Commands and arguments
#
CC = gcc
RM = rm

# ADD -m32 ON 64BIT PLATFORMS
CFLAGS = -Wall -g -std=c99 -pedantic

# ADD -32 ON 64BIT PLATFORMS
ASFLAGS = -g


all: test test_arg test_ret


# The simple test program
test: sthread.o glue.o test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# test_arg program to test passing argument to thread functions
test_arg: sthread.o glue.o test_arg.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# test_ret program to test threads running for different lengths of time
test_ret: sthread.o glue.o test_ret.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# pseudo-target to clean up
clean:
	$(RM) -f *.o core* *~ test test_arg test_ret


.PHONY: all clean


# Dependencies
sthread.c: sthread.h
test.c: sthread.h


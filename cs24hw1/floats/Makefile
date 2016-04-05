#=============================================================================#
# makefile for building the fsum program.
#
# Caltech CS24 - Introduction to Computing Systems
# Spring 2011 - Donnie Pinkston (donnie@cs.caltech.edu)
#=============================================================================#

# Use the GNU compiler gcc
CC=gcc

# Report all warnings, and report them as errors.  Warnings should *always*
# be fixed.
CFLAGS = -Wall -Werror
CFLAGS += -Wno-error=unused-but-set-variable


#=============================================================================#
# These are the actual build rules.  We rely on implicit build rules to keep
# our file short.

all: fsum

fsum: fsum.o ffunc.o
	$(CC) $(CFLAGS) -o fsum fsum.o ffunc.o $(LDFLAGS)

clean:
	rm -f fsum *.o *~

.PHONY: all clean


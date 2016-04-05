#=============================================================================#
# Simple makefile for building the onebits program.
#
# This makefile doesn't use implicit build rules, although you are welcome to
# use them if you know how.
#
# Caltech CS24 - Introduction to Computing Systems
# Spring 2009 - Donnie Pinkston (donnie@cs.caltech.edu)
#=============================================================================#

# The first build rule is used if no arguments are specfiied to make.
# Alternatively, one can type "make clean" to run the clean rule, or
# "make clean all" to invoke multiple build targets.

all: onebits faster_onebits

# This rule specifies how to generate the onebits program, if we also have
# onebits.o.  If onebits.o doesn't exist, make will use the rule for onebits.o
# to generate it from onebits.c.

onebits: onebits.o
	gcc -Wall -Werror -o onebits onebits.o

onebits.o: onebits.c
	gcc -Wall -Werror -c onebits.c

# [Implemented by Joon] Rules for faster_onebits

faster_onebits: faster_onebits.o
	gcc -Wall -Werror -o faster_onebits faster_onebits.o

faster_onebits.o: faster_onebits.c
	gcc -Wall -Werror -c faster_onebits.c

# Clean up all files generated during the build process.
# BE VERY CAREFUL editing this rule; don't delete your souce code!

clean:
	rm -f onebits faster_onebits *.o *~

# This build rule specifies all build targets that are not actual files.  Other
# rules actually generate a file with the same name as the target, but the "all"
# and "clean" rules do not generate files with these names.

.PHONY: all clean

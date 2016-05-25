#
# Compiler configuration
#

# Add -m32 on 64bit platforms.
CFLAGS = -Wall -g

# Add -32 on 64bit platforms.
ASFLAGS = -g

# Object files:
OFILES = glue.o sthread.o timer.o semaphore.o fibtest.o bounded_buffer.o


#
# How to build the program
#

all: fibtest


fibtest: $(OFILES)
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o *~ fibtest


.PHONY: all clean


#
# Dependencies
#
timer.o: timer.h glue.h
sthread.o: sthread.h glue.h
bounded_buffer.o: bounded_buffer.h sthread.h glue.h
fibtest.o: sthread.h bounded_buffer.h


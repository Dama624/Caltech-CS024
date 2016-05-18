CC = gcc
CFLAGS = -Wall -g -O0

OBJS = myids.o get_ids.o
EXEC = myids


all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *~ *.o $(EXEC) $(EXEC).exe

.PHONY:  clean all


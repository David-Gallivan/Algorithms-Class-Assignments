#JMJ

CC = gcc
CFLAGS = -Wall -g -O0
ALL = ow

all: $(ALL)

ow: main.o heap.o
	$(CC) $(CFLAGS) main.o -o "ow"

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

run: all
	./ow

clean:
	rm -f $(ALL) *.o

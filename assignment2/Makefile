#JMJ

CC = g++
CFLAGS = -Wall -g -O0 #FIXME change optimization
ALL = tttdemo

all: $(ALL)

tttdemo: usettt.o ttt.o
	$(CC) $(CFLAGS) usettt.o ttt.o -o tttdemo

usettt.o: usettt.cpp ttt.h
	$(CC) $(CFLAGS) -c usettt.cpp

ttt.o: ttt.h ttt.cpp
	$(CC) $(CFLAGS) -c ttt.cpp

run: all
	./tttdemo

clean:
	rm -f $(ALL) *.o

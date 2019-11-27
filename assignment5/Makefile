#JMJ

all: markovomatic

markovomatic: mainkov.c
	gcc -O3 mainkov.c -o markovomatic

dbg: mainkov.c
	gcc -Wall -g -O0 mainkov.c -o "dbg_markovomatic"

run: all
	./markovomatic

clean:
	rm -f *markovomatic

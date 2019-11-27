#JMJ

CC = gcc
CFLAGS = -Wall -g
GOAL = SecSim SecSim-EC SecSim-FT

all: $(GOAL)
	
SecSim: main.o officeQueue.o
	$(CC) $(CFLAGS) -o SecSim main.o officeQueue.o

SecSim-EC: ecmain.o officeQueue.o
	$(CC) $(CFLAGS) -o SecSim-EC ecmain.o officeQueue.o

SecSim-FT: ftmain.o officeQueue.o
	$(CC) $(CFLAGS) -o SecSim-FT ftmain.o officeQueue.o

main.o: main.c officeQueue.h
	$(CC) $(CFLAGS) -c main.c

ecmain.o: ecmain.c officeQueue.h
	$(CC) $(CFLAGS) -c ecmain.c

ftmain.0: ftmain.c officeQueue.h
	$(CC) $(CFLAGS) -c ftmain.c

officeQueue.o: officeQueue.c officeQueue.h
	$(CC) $(CFLAGS) -c officeQueue.*

run: SecSim
	./SecSim

ec: SecSim-EC
	./SecSim-EC

ft: SecSim-FT
	./SecSim-FT

clean:
	rm -f $(GOAL) *.o *.gch

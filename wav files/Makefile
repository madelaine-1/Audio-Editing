CC=gcc
CFLAGS=-I.

all: main

main: main.o wavParser.o
	$(CC) -o main main.o wavParser.o

main.o: main.c wav.h
	$(CC) -c main.c

wavParser.o: wavParser.c wav.h
	$(CC) -c wavParser.c

clean:
	rm -f *.o main
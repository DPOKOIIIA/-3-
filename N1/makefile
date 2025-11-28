CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

all: main test

main: main.o functions.o
	$(CC) $(CFLAGS) -o main main.o functions.o

test: test.o functions.o
	$(CC) $(CFLAGS) -o test test.o functions.o

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

test.o: test.c functions.h
	$(CC) $(CFLAGS) -c test.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o main test

.PHONY: all clean
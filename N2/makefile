CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -fsanitize=address -Werror

all: main test_program

main: main.o functions.o
	$(CC) $(CFLAGS) -o main main.o functions.o

test_program: test.o functions.o
	$(CC) $(CFLAGS) -o test_program test.o functions.o

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

test.o: test.c functions.h
	$(CC) $(CFLAGS) -c test.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o main test_program

test: test_program
	./test_program

.PHONY: all clean test
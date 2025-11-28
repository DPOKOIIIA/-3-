CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: main test

main: main.c functions.c functions.h
	$(CC) $(CFLAGS) -o main main.c functions.c

test: test.c functions.c functions.h
	$(CC) $(CFLAGS) -o test test.c functions.c

clean:
	rm -f main test

.PHONY: all clean test
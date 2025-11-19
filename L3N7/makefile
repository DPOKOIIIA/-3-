CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
TARGET = interpreter
SOURCES = main.c functions.c
TEST_SOURCES = test.c functions.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o test_program $^
	./test_program

clean:
	rm -f $(TARGET) test_program trace.log

.PHONY: all test clean
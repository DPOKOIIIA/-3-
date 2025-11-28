CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
MAIN_TARGET = main
TEST_TARGET = test_system

SOURCES = main.c functions.c
TEST_SOURCES = test.c functions.c

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS += -D_POSIX_C_SOURCE=200112L
endif
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -D_DARWIN_C_SOURCE
endif

all: $(MAIN_TARGET) $(TEST_TARGET)

$(MAIN_TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(MAIN_TARGET) $(SOURCES) -lm

$(TEST_TARGET): $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES) -lm

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) *.txt *.log

.PHONY: all test clean
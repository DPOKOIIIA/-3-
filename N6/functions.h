#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    BRACKETS_OK = 0,
    BRACKETS_UNBALANCED = 1,
    BRACKETS_MISMATCH = 2,
    BRACKETS_STACK_OVERFLOW = 3,
    BRACKETS_STACK_UNDERFLOW = 4,
    BRACKETS_NULL_PTR = 5
} BracketStatus;

BracketStatus check_brackets(const char *str);

#endif
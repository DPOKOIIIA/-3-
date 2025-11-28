#include <assert.h>
#include <stdio.h>
#include "functions.h"

void run_tests() {
    assert(check_brackets("") == BRACKETS_OK);
    assert(check_brackets("()") == BRACKETS_OK);
    assert(check_brackets("({[<>]})") == BRACKETS_OK);
    assert(check_brackets("({})") == BRACKETS_OK);
    assert(check_brackets("<>") == BRACKETS_OK);
    assert(check_brackets("((()))") == BRACKETS_OK);
    assert(check_brackets("no brackets") == BRACKETS_OK);

    assert(check_brackets("(") == BRACKETS_UNBALANCED);
    assert(check_brackets(")") == BRACKETS_STACK_UNDERFLOW);
    assert(check_brackets("([)]") == BRACKETS_MISMATCH);
    assert(check_brackets("({)}") == BRACKETS_MISMATCH);
    assert(check_brackets("((()") == BRACKETS_UNBALANCED);
    assert(check_brackets("><") == BRACKETS_STACK_UNDERFLOW);
    assert(check_brackets("({})>") == BRACKETS_STACK_UNDERFLOW);

    assert(check_brackets(NULL) == BRACKETS_NULL_PTR);

    printf("Все тесты пройдены успешно!\n");
}

int main() {
    run_tests();
    return 0;
}
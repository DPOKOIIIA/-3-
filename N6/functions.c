#include "functions.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *data;
    int top;
    int capacity;
} Stack;

static void init_stack(Stack *s, int capacity) {
    s->data = (char*)malloc(capacity * sizeof(char));
    s->top = -1;
    s->capacity = capacity;
}

static void free_stack(Stack *s) {
    free(s->data);
}

static int is_empty(Stack *s) {
    return s->top == -1;
}

static BracketStatus push(Stack *s, char c) {
    s->data[++(s->top)] = c;
    return BRACKETS_OK;
}

static BracketStatus pop(Stack *s, char *out) {
    if (is_empty(s)) return BRACKETS_STACK_UNDERFLOW;
    *out = s->data[(s->top)--];
    return BRACKETS_OK;
}

static char get_closing_bracket(char open) {
    switch (open) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
        default: return 0;
    }
}

BracketStatus check_brackets(const char *str) {
    if (str == NULL) return BRACKETS_NULL_PTR;
    
    int len = strlen(str);
    Stack stack;
    init_stack(&stack, len);

    char c;
    BracketStatus status;

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];
        if (current == '(' || current == '[' || current == '{' || current == '<') {
            push(&stack, current);
        } else if (current == ')' || current == ']' || current == '}' || current == '>') {
            status = pop(&stack, &c);
            if (status != BRACKETS_OK) {
                free_stack(&stack);
                return status;
            }
            
            char expected = get_closing_bracket(c);
            if (expected != current) {
                free_stack(&stack);
                return BRACKETS_MISMATCH;
            }
        }
    }
    
    status = is_empty(&stack) ? BRACKETS_OK : BRACKETS_UNBALANCED;
    free_stack(&stack);
    return status;
}
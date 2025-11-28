#include <stdio.h>
#include "functions.h"

const char* status_to_string(BracketStatus status) {
    switch (status) {
        case BRACKETS_OK: return "Корректно";
        case BRACKETS_UNBALANCED: return "Некорректно: несбалансированные скобки";
        case BRACKETS_MISMATCH: return "Некорректно: несоответствие типов скобок";
        case BRACKETS_STACK_OVERFLOW: return "Некорректно: переполнение стека";
        case BRACKETS_STACK_UNDERFLOW: return "Некорректно: лишние закрывающие скобки";
        case BRACKETS_NULL_PTR: return "Некорректно: NULL указатель";
        default: return "Некорректно: неизвестная ошибка";
    }
}

int main() {
    const char *examples[] = {
        "()", 
        "({[<>]})", 
        "([)]", 
        "({})", 
        "<>", 
        "((()))", 
        "no brackets", 
        "(", 
        "(((.)(.))типо сиськи)",
        ""
    };
    int num_examples = sizeof(examples) / sizeof(examples[0]);

    for (int i = 0; i < num_examples; i++) {
        printf("Строка: \"%s\"\n", examples[i]);
        BracketStatus result = check_brackets(examples[i]);
        printf("Результат: %s\n\n", status_to_string(result));
    }
    return 0;
}
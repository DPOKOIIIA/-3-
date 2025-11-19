#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "functions.h"

void test_init_variables() {
    VariablesState vars;
    StatusCode status = init_variables(&vars);
    assert(status == SUCCESS);
    
    for (int i = 0; i < 26; i++) {
        assert(vars.values[i] == 0);
        assert(vars.initialized[i] == 0);
    }
    
    status = init_variables(NULL);
    assert(status == ERROR_NULL_POINTER);
    
    printf("test_init_variables passed\n");
}

void test_eval_expression() {
    int variables[26] = {0};
    variables['A' - 'A'] = 2;
    variables['B' - 'A'] = 3;
    int pos;
    int result;
    StatusCode status;
    
    pos = 0;
    status = eval_expression("5", &pos, variables, &result);
    assert(status == SUCCESS);
    assert(result == 5);
    
    pos = 0;
    status = eval_expression("A", &pos, variables, &result);
    assert(status == SUCCESS);
    assert(result == 2);
    
    pos = 0;
    status = eval_expression("A + B", &pos, variables, &result);
    assert(status == SUCCESS);
    assert(result == 5);
    
    pos = 0;
    status = eval_expression("A + B * 2", &pos, variables, &result);
    assert(status == SUCCESS);
    assert(result == 8);
    
    pos = 0;
    status = eval_expression("A ^ B", &pos, variables, &result);
    assert(status == SUCCESS);
    assert(result == 8);
    
    pos = 0;
    status = eval_expression("A * B + 2", &pos, variables, &result);
    assert(status == SUCCESS);
    assert(result == 8);
    
    status = eval_expression(NULL, &pos, variables, &result);
    assert(status == ERROR_NULL_POINTER);
    
    printf("test_eval_expression passed\n");
}

void test_get_operation_type() {
    assert(strcmp(get_operation_type("5"), "Assignment") == 0);
    assert(strcmp(get_operation_type("A"), "Assignment") == 0);
    assert(strcmp(get_operation_type("A + B"), "Arithmetic operation") == 0);
    assert(strcmp(get_operation_type("A * B"), "Arithmetic operation") == 0);
    assert(strcmp(get_operation_type("A ^ B"), "Arithmetic operation") == 0);
    assert(strcmp(get_operation_type("A - B"), "Arithmetic operation") == 0);
    assert(strcmp(get_operation_type(NULL), "Assignment") == 0);
    
    printf("test_get_operation_type passed\n");
}

int main() {
    test_init_variables();
    test_eval_expression();
    test_get_operation_type();
    
    printf("All basic tests passed!\n");
    return 0;
}
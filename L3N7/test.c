#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
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

void test_process_single_command() {
    VariablesState vars;
    init_variables(&vars);
    FILE* trace_file = fopen("test_trace.log", "w");
    assert(trace_file != NULL);
    
    StatusCode status = process_single_command("A = 5", 1, &vars, trace_file);
    assert(status == SUCCESS);
    assert(vars.values['A' - 'A'] == 5);
    assert(vars.initialized['A' - 'A'] == 1);
    
    status = process_single_command("B = A + 3", 2, &vars, trace_file);
    assert(status == SUCCESS);
    assert(vars.values['B' - 'A'] == 8);
    assert(vars.initialized['B' - 'A'] == 1);
    
    status = process_single_command("print(B)", 3, &vars, trace_file);
    assert(status == SUCCESS);
    
    fclose(trace_file);
    remove("test_trace.log");
    
    printf("test_process_single_command passed\n");
}

void test_process_line_without_delimiter() {
    VariablesState vars;
    init_variables(&vars);
    FILE* trace_file = fopen("test_trace.log", "w");
    assert(trace_file != NULL);
    
    StatusCode status = process_line_without_delimiter("A = 5 B = A + 3 print(B)", 1, &vars, trace_file);
    assert(status == SUCCESS);
    assert(vars.values['A' - 'A'] == 5);
    assert(vars.values['B' - 'A'] == 8);
    
    fclose(trace_file);
    
    // Проверяем содержимое лог-файла
    trace_file = fopen("test_trace.log", "r");
    char buffer[256];
    int assignment_count = 0;
    int arithmetic_count = 0;
    int print_count = 0;
    
    while (fgets(buffer, sizeof(buffer), trace_file)) {
        if (strstr(buffer, "Assignment")) assignment_count++;
        else if (strstr(buffer, "Arithmetic operation")) arithmetic_count++;
        else if (strstr(buffer, "Print")) print_count++;
    }
    
    fclose(trace_file);
    remove("test_trace.log");
    
    assert(assignment_count == 1); // A = 5
    assert(arithmetic_count == 1); // B = A + 3
    assert(print_count == 1);      // print(B)
    
    printf("test_process_line_without_delimiter passed\n");
}

void test_multiple_commands_complex() {
    VariablesState vars;
    init_variables(&vars);
    FILE* trace_file = fopen("test_trace.log", "w");
    assert(trace_file != NULL);
    
    StatusCode status = process_line_without_delimiter("A = 2 B = 3 C = A + B * 2 D = C ^ B print(D)", 1, &vars, trace_file);
    assert(status == SUCCESS);
    
    assert(vars.values['A' - 'A'] == 2);
    assert(vars.values['B' - 'A'] == 3);
    assert(vars.values['C' - 'A'] == 8);
    assert(vars.values['D' - 'A'] == 512);
    
    fclose(trace_file);
    
    trace_file = fopen("test_trace.log", "r");
    char buffer[256];
    int line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), trace_file)) {
        line_count++;
    }
    
    fclose(trace_file);
    remove("test_trace.log");
    
    assert(line_count == 5); // 4 присваивания + 1 print
    
    printf("test_multiple_commands_complex passed\n");
}

void test_expression_in_print() {
    VariablesState vars;
    init_variables(&vars);
    FILE* trace_file = fopen("test_trace.log", "w");
    assert(trace_file != NULL);
    
    StatusCode status = process_line_without_delimiter("A = 5 B = 3 print(A + B * 2)", 1, &vars, trace_file);
    assert(status == SUCCESS);
    assert(vars.values['A' - 'A'] == 5);
    assert(vars.values['B' - 'A'] == 3);
    
    fclose(trace_file);
    remove("test_trace.log");
    
    printf("test_expression_in_print passed\n");
}

int main() {
    test_init_variables();
    test_eval_expression();
    test_get_operation_type();
    test_process_single_command();
    test_process_line_without_delimiter();
    test_multiple_commands_complex();
    test_expression_in_print();
    
    printf("All tests passed!\n");
    return 0;
}
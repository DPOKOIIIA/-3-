#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER,
    ERROR_INVALID_VARIABLE,
    ERROR_INVALID_EXPRESSION,
    ERROR_FILE_OPEN,
    ERROR_SYNTAX,
    ERROR_MEMORY,
    ERROR_INVALID_COMMAND
} StatusCode;

typedef struct {
    int values[26];
    int initialized[26];
} VariablesState;

StatusCode init_variables(VariablesState* vars);
StatusCode eval_expression(const char* str, int* pos, const int* variables, int* result);
StatusCode log_command(FILE* trace_file, int line_num, const char* command, const VariablesState* vars, const char* operation);

StatusCode process_single_command(const char* command, int line_num, VariablesState* vars, FILE* trace_file);
StatusCode process_line_without_delimiter(const char* line, int line_num, VariablesState* vars, FILE* trace_file);

void skip_spaces(const char* str, int* pos);
const char* get_operation_type(const char* expr);

#endif
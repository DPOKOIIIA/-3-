#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

StatusCode init_variables(VariablesState* vars) {
    if (vars == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    for (int i = 0; i < 26; i++) {
        vars->values[i] = 0;
        vars->initialized[i] = 0;
    }
    
    return SUCCESS;
}

void skip_spaces(const char* str, int* pos) {
    if (str == NULL || pos == NULL) return;
    
    while (str[*pos] == ' ') {
        (*pos)++;
    }
}

int power_int(int base, int exp) {
    if (exp < 0) return 0;
    if (exp == 0) return 1;
    int result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

StatusCode eval_base(const char* str, int* pos, const int* variables, int* result) {
    if (str == NULL || pos == NULL || variables == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    skip_spaces(str, pos);
    
    if (isdigit(str[*pos])) {
        *result = 0;
        while (isdigit(str[*pos])) {
            *result = *result * 10 + (str[*pos] - '0');
            (*pos)++;
        }
    } else if (isupper(str[*pos])) {
        int var_index = str[*pos] - 'A';
        *result = variables[var_index];
        (*pos)++;
    } else {
        *result = 0;
        return ERROR_INVALID_EXPRESSION;
    }
    
    skip_spaces(str, pos);
    return SUCCESS;
}

StatusCode eval_factor(const char* str, int* pos, const int* variables, int* result) {
    if (str == NULL || pos == NULL || variables == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    StatusCode status = eval_base(str, pos, variables, result);
    if (status != SUCCESS) {
        return status;
    }
    
    skip_spaces(str, pos);
    if (str[*pos] == '^') {
        (*pos)++;
        skip_spaces(str, pos);
        int exponent;
        status = eval_factor(str, pos, variables, &exponent);
        if (status != SUCCESS) {
            return status;
        }
        *result = power_int(*result, exponent);
    }
    
    return SUCCESS;
}

StatusCode eval_term(const char* str, int* pos, const int* variables, int* result) {
    if (str == NULL || pos == NULL || variables == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    StatusCode status = eval_factor(str, pos, variables, result);
    if (status != SUCCESS) {
        return status;
    }
    
    skip_spaces(str, pos);
    while (str[*pos] == '*') {
        (*pos)++;
        skip_spaces(str, pos);
        int right;
        status = eval_factor(str, pos, variables, &right);
        if (status != SUCCESS) {
            return status;
        }
        *result *= right;
        skip_spaces(str, pos);
    }
    
    return SUCCESS;
}

StatusCode eval_expression(const char* str, int* pos, const int* variables, int* result) {
    if (str == NULL || pos == NULL || variables == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    StatusCode status = eval_term(str, pos, variables, result);
    if (status != SUCCESS) {
        return status;
    }
    
    skip_spaces(str, pos);
    while (str[*pos] == '+' || str[*pos] == '-') {
        char op = str[*pos];
        (*pos)++;
        skip_spaces(str, pos);
        int right;
        status = eval_term(str, pos, variables, &right);
        if (status != SUCCESS) {
            return status;
        }
        if (op == '+') {
            *result += right;
        } else {
            *result -= right;
        }
        skip_spaces(str, pos);
    }
    
    return SUCCESS;
}

const char* get_operation_type(const char* expr) {
    if (expr == NULL) {
        return "Assignment";
    }
    
    int pos = 0;
    skip_spaces(expr, &pos);
    
    if (expr[pos] == '\0') {
        return "Assignment";
    }
    
    while (expr[pos] != '\0') {
        if (expr[pos] == '+' || expr[pos] == '-' || expr[pos] == '*' || expr[pos] == '^') {
            return "Arithmetic operation";
        }
        pos++;
    }
    
    return "Assignment";
}

StatusCode log_command(FILE* trace_file, int line_num, const char* command, const VariablesState* vars, const char* operation) {
    if (trace_file == NULL || command == NULL || vars == NULL || operation == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    fprintf(trace_file, "[%d] %s | ", line_num, command);
    int first = 1;
    for (int i = 0; i < 26; i++) {
        if (vars->initialized[i]) {
            if (!first) {
                fprintf(trace_file, ", ");
            }
            fprintf(trace_file, "%c=%d", 'A' + i, vars->values[i]);
            first = 0;
        }
    }
    fprintf(trace_file, " | %s\n", operation);
    
    return SUCCESS;
}
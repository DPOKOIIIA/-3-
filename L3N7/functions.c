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

StatusCode process_single_command(const char* command, int line_num, VariablesState* vars, FILE* trace_file) {
    if (command == NULL || vars == NULL || trace_file == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    int pos = 0;
    skip_spaces(command, &pos);
    
    if (strncmp(command + pos, "print", 5) == 0) {
        pos += 5;
        skip_spaces(command, &pos);
        if (command[pos] != '(') {
            return ERROR_SYNTAX;
        }
        pos++;
        skip_spaces(command, &pos);
        
        int expr_start = pos;
        while (command[pos] != '\0' && command[pos] != ')') {
            pos++;
        }
        if (command[pos] != ')') {
            return ERROR_SYNTAX;
        }
        
        char expr[256];
        int expr_len = pos - expr_start;
        if (expr_len >= 256) {
            return ERROR_INVALID_EXPRESSION;
        }
        strncpy(expr, command + expr_start, expr_len);
        expr[expr_len] = '\0';
        
        int value;
        int expr_pos = 0;
        StatusCode status = eval_expression(expr, &expr_pos, vars->values, &value);
        if (status != SUCCESS) {
            return status;
        }
        
        printf("%d\n", value);
        
        char full_command[256];
        int command_len = pos + 1 - (command - command);
        if (command_len >= 256) {
            command_len = 255;
        }
        strncpy(full_command, command, command_len);
        full_command[command_len] = '\0';
        
        return log_command(trace_file, line_num, full_command, vars, "Print");
        
    } else if (isupper(command[pos])) {
        char var = command[pos];
        int var_index = var - 'A';
        pos++;
        skip_spaces(command, &pos);
        
        if (command[pos] != '=') {
            return ERROR_SYNTAX;
        }
        pos++;
        skip_spaces(command, &pos);
        
        int expr_start = pos;
        while (command[pos] != '\0') {
            if (isupper(command[pos])) {
                int j = pos + 1;
                skip_spaces(command, &j);
                if (command[j] == '=') {
                    break;
                }
            } else if (strncmp(command + pos, "print", 5) == 0) {
                int j = pos + 5;
                skip_spaces(command, &j);
                if (command[j] == '(') {
                    break;
                }
            }
            pos++;
        }
        
        char expr[256];
        int expr_len = pos - expr_start;
        if (expr_len >= 256) {
            return ERROR_INVALID_EXPRESSION;
        }
        strncpy(expr, command + expr_start, expr_len);
        expr[expr_len] = '\0';
        
        int value;
        int expr_pos = 0;
        StatusCode status = eval_expression(expr, &expr_pos, vars->values, &value);
        if (status != SUCCESS) {
            return status;
        }
        
        vars->values[var_index] = value;
        vars->initialized[var_index] = 1;
        
        char full_command[256];
        int command_len = pos - (command - command);
        if (command_len >= 256) {
            command_len = 255;
        }
        strncpy(full_command, command, command_len);
        full_command[command_len] = '\0';
        
        const char* operation_type = get_operation_type(expr);
        return log_command(trace_file, line_num, full_command, vars, operation_type);
        
    } else {
        return ERROR_INVALID_COMMAND;
    }
}

StatusCode process_line_without_delimiter(const char* line, int line_num, VariablesState* vars, FILE* trace_file) {
    if (line == NULL || vars == NULL || trace_file == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    char* line_copy = malloc(strlen(line) + 1);
    if (line_copy == NULL) {
        return ERROR_MEMORY;
    }
    strcpy(line_copy, line);
    
    int i = 0;
    int len = strlen(line_copy);
    
    while (i < len) {
        skip_spaces(line_copy, &i);
        if (i >= len) break;
        
        int command_start = i;
        
        if (strncmp(&line_copy[i], "print", 5) == 0 && (i + 5 < len) && line_copy[i + 5] == '(') {
            i += 5;
            i++;
            
            while (i < len && line_copy[i] != ')') {
                i++;
            }
            if (i >= len) {
                free(line_copy);
                return ERROR_SYNTAX;
            }
            i++;
            
        } else if (isupper(line_copy[i])) {
            i++;
            skip_spaces(line_copy, &i);
            if (i >= len || line_copy[i] != '=') {
                free(line_copy);
                return ERROR_SYNTAX;
            }
            i++;
            skip_spaces(line_copy, &i);
            
            while (i < len) {
                if (isupper(line_copy[i])) {
                    int j = i + 1;
                    skip_spaces(line_copy, &j);
                    if (j < len && line_copy[j] == '=') {
                        break;
                    }
                } else if (strncmp(&line_copy[i], "print", 5) == 0 && (i + 5 < len) && line_copy[i + 5] == '(') {
                    break;
                }
                i++;
            }
        } else {
            free(line_copy);
            return ERROR_INVALID_COMMAND;
        }
        
        char command[256];
        int command_len = i - command_start;
        if (command_len >= 256) {
            free(line_copy);
            return ERROR_INVALID_COMMAND;
        }
        strncpy(command, &line_copy[command_start], command_len);
        command[command_len] = '\0';
        
        StatusCode status = process_single_command(command, line_num, vars, trace_file);
        if (status != SUCCESS) {
            free(line_copy);
            return status;
        }
    }
    
    free(line_copy);
    return SUCCESS;
}
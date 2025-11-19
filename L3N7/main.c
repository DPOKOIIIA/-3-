#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"


StatusCode process_single_line(const char* line, int line_num, VariablesState* vars, FILE* trace_file) {
    if (line == NULL || vars == NULL || trace_file == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    int pos = 0;
    skip_spaces(line, &pos);
    
    if (strncmp(line + pos, "print", 5) == 0) {
        pos += 5;
        skip_spaces(line, &pos);
        if (line[pos] != '(') return ERROR_SYNTAX;
        pos++;
        
        if (!isupper(line[pos])) return ERROR_INVALID_VARIABLE;
        char var = line[pos];
        pos++;
        
        if (line[pos] != ')') return ERROR_SYNTAX;
        
        if (!vars->initialized[var - 'A']) return ERROR_INVALID_VARIABLE;
        
        printf("%d\n", vars->values[var - 'A']);
        return log_command(trace_file, line_num, line, vars, "Print");
        
    } else if (isupper(line[pos])) {
        char var = line[pos];
        int var_index = var - 'A';
        pos++;
        skip_spaces(line, &pos);
        
        if (line[pos] != '=') return ERROR_SYNTAX;
        pos++;
        skip_spaces(line, &pos);
        
        int value;
        StatusCode status = eval_expression(line, &pos, vars->values, &value);
        if (status != SUCCESS) return status;
        
        vars->values[var_index] = value;
        vars->initialized[var_index] = 1;
        
        const char* operation_type = get_operation_type(line + pos);
        return log_command(trace_file, line_num, line, vars, operation_type);
    }
    
    return ERROR_INVALID_COMMAND;
}

int main() {
    VariablesState vars;
    StatusCode status = init_variables(&vars);
    if (status != SUCCESS) {
        fprintf(stderr, "Error initializing variables: %d\n", status);
        return 1;
    }

    FILE* trace_file = fopen("trace.log", "w");
    if (!trace_file) {
        perror("Failed to open trace.log");
        return 1;
    }

    char line[256];
    int line_num = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;
        
        status = process_single_line(line, line_num, &vars, trace_file);
        if (status != SUCCESS) {
            fprintf(stderr, "Error processing line %d: %d\n", line_num, status);
        }
        
        line_num++;
    }

    fclose(trace_file);
    return 0;
}
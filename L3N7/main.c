#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

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
        
        status = process_line_without_delimiter(line, line_num, &vars, trace_file);
        if (status != SUCCESS) {
            fprintf(stderr, "Error processing line %d: %d\n", line_num, status);
        }
        
        line_num++;
    }

    fclose(trace_file);
    return 0;
}
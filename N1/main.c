#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void demo_conversion(unsigned int number, int r) {
    char* result = NULL;
    ConversionStatus status = convert_to_base(number, r, &result);
    
    if (status == CONVERSION_OK && result != NULL) {
        printf("Number %u in base 2^%d (%d) = %s\n", 
               number, r, 1 << r, result);
        free(result);
    } else {
        printf("Error converting number %u with base 2^%d: status code %d\n", 
               number, r, status);
    }
}

int main() {
    unsigned int test_numbers[] = {0, 1, 15, 16, 255, 1024, 12345};
    int r_values[] = {1, 2, 3, 4, 5};
    
    int test_numbers_count = sizeof(test_numbers) / sizeof(test_numbers[0]);
    int r_values_count = sizeof(r_values) / sizeof(r_values[0]);
    
    printf("Number conversion demonstration:\n\n");
    
    for (int i = 0; i < test_numbers_count; i++) {
        for (int j = 0; j < r_values_count; j++) {
            demo_conversion(test_numbers[i], r_values[j]);
        }
        printf("\n");
    }
    
    printf("Error handling demonstration:\n");
    
    char* result = NULL;
    ConversionStatus status;
    
    status = convert_to_base(10, 0, &result);
    printf("convert_to_base(10, 0, &result) -> status: %d\n", status);
    
    status = convert_to_base(10, 6, &result);
    printf("convert_to_base(10, 6, &result) -> status: %d\n", status);
    
    status = convert_to_base(10, 2, NULL);
    printf("convert_to_base(10, 2, NULL) -> status: %d\n", status);
    
    return 0;
}
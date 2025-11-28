#include <stdlib.h>
#include "functions.h"

ConversionStatus convert_to_base(const unsigned int n, const int r, char** result) {
    if (result == NULL) {
        return CONVERSION_NULL_POINTER;
    }
    
    if (r < 1 || r > 5) {
        return CONVERSION_INVALID_BASE;
    }
    
    unsigned int base = 1;
    int i = 0;
    while (i < r) {
        base = base << 1;
        i++;
    }

    if (n == 0) {
        *result = (char*)malloc(2 * sizeof(char));
        if (*result == NULL) {
            return CONVERSION_MEMORY_ERROR;
        }
        (*result)[0] = '0';
        (*result)[1] = '\0';
        return CONVERSION_OK;
    }

    unsigned int temp = n;
    int digits_count = 0;
    while (temp != 0) {
        temp = temp >> r;
        digits_count++;
    }

    const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    
    *result = (char*)malloc((digits_count + 1) * sizeof(char));
    if (*result == NULL) {
        return CONVERSION_MEMORY_ERROR;
    }
    (*result)[digits_count] = '\0';

    temp = n;
    int index = digits_count - 1;
    while (index >= 0) {
        unsigned int digit = temp & (base - 1);
        (*result)[index] = alphabet[digit];
        temp = temp >> r;
        index--;
    }

    return CONVERSION_OK;
}
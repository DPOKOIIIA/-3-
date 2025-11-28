#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
    CONVERSION_OK = 0,
    CONVERSION_INVALID_BASE = 1,
    CONVERSION_MEMORY_ERROR = 2,
    CONVERSION_NULL_POINTER = 3
} ConversionStatus;

ConversionStatus convert_to_base(const unsigned int n, const int r, char** result);

#endif
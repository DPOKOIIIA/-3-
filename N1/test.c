#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int tests_passed = 0;
int tests_failed = 0;

void run_test(const char* test_name, int (*test_func)()) {
    if (test_func()) {
        printf("PASS: Test '%s'\n", test_name);
        tests_passed++;
    } else {
        printf("FAIL: Test '%s'\n", test_name);
        tests_failed++;
    }
}

int test_zero() {
    char* result = NULL;
    ConversionStatus status = convert_to_base(0, 2, &result);
    int success = (status == CONVERSION_OK && strcmp(result, "0") == 0);
    free(result);
    return success;
}

int test_base_2() {
    char* result = NULL;
    ConversionStatus status = convert_to_base(10, 1, &result);
    int success = (status == CONVERSION_OK && strcmp(result, "1010") == 0);
    free(result);
    return success;
}

int test_base_4() {
    char* result = NULL;
    ConversionStatus status = convert_to_base(10, 2, &result);
    int success = (status == CONVERSION_OK && strcmp(result, "22") == 0);
    free(result);
    return success;
}

int test_base_8() {
    char* result = NULL;
    ConversionStatus status = convert_to_base(10, 3, &result);
    int success = (status == CONVERSION_OK && strcmp(result, "12") == 0);
    free(result);
    return success;
}

int test_base_16() {
    char* result = NULL;
    ConversionStatus status = convert_to_base(255, 4, &result);
    int success = (status == CONVERSION_OK && strcmp(result, "FF") == 0);
    free(result);
    return success;
}

int test_base_32() {
    char* result = NULL;
    ConversionStatus status = convert_to_base(1024, 5, &result);
    int success = (status == CONVERSION_OK && strcmp(result, "100") == 0);
    free(result);
    return success;
}

int test_invalid_r() {
    char* result = NULL;
    ConversionStatus status;
    
    status = convert_to_base(10, 0, &result);
    int success1 = (status == CONVERSION_INVALID_BASE && result == NULL);
    
    status = convert_to_base(10, 6, &result);
    int success2 = (status == CONVERSION_INVALID_BASE && result == NULL);
    
    return success1 && success2;
}

int test_null_pointer() {
    ConversionStatus status = convert_to_base(10, 2, NULL);
    return status == CONVERSION_NULL_POINTER;
}

int main() {
    printf("Running tests:\n\n");
    
    run_test("Zero value", test_zero);
    run_test("Base 2 (binary)", test_base_2);
    run_test("Base 4", test_base_4);
    run_test("Base 8", test_base_8);
    run_test("Base 16", test_base_16);
    run_test("Base 32", test_base_32);
    run_test("Invalid r parameter", test_invalid_r);
    run_test("NULL pointer", test_null_pointer);
    
    printf("\nTest results:\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total tests: %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0) {
        printf("\nAll tests passed successfully!\n");
        return 0;
    } else {
        printf("\nSome tests failed!\n");
        return 1;
    }
}
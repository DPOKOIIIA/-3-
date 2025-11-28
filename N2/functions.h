#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>

#ifndef VECTOR_TYPE
#define VECTOR_TYPE int
#endif

typedef enum {
    VECTOR_SUCCESS = 0,
    VECTOR_ERROR_NULL_POINTER,
    VECTOR_ERROR_MEMORY_ALLOCATION,
    VECTOR_ERROR_INDEX_OUT_OF_BOUNDS,
    VECTOR_ERROR_EMPTY_VECTOR,
    VECTOR_ERROR_INVALID_CAPACITY
} VectorStatus;

typedef struct {
    VECTOR_TYPE *data;
    size_t size;
    size_t capacity;
    VECTOR_TYPE (*CopyVoidPtr)(VECTOR_TYPE);
    void (*DeleteVoidPtr)(VECTOR_TYPE);
} Vector;

VectorStatus create_vector(Vector *vec, size_t initial_capacity, 
                          VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE), 
                          void (*DeleteFunc)(VECTOR_TYPE));
VectorStatus erase_vector(Vector *v);
int is_equal_vector(const Vector *v1, const Vector *v2);
VectorStatus copy_vector(Vector *dest, const Vector *src);
VectorStatus copy_vector_new(const Vector *src, Vector **result);
VectorStatus push_back_vector(Vector *v, VECTOR_TYPE value);
VectorStatus delete_at_vector(Vector *v, size_t index, VECTOR_TYPE *deleted_value);
VectorStatus get_at_vector(const Vector *v, size_t index, VECTOR_TYPE *result);
VectorStatus delete_vector(Vector *v);

const char* vector_status_string(VectorStatus status);

#endif
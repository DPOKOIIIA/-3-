#include "functions.h"
#include <stdlib.h>
#include <string.h>

const char* vector_status_string(VectorStatus status) {
    switch (status) {
        case VECTOR_SUCCESS: return "Success";
        case VECTOR_ERROR_NULL_POINTER: return "Null pointer error";
        case VECTOR_ERROR_MEMORY_ALLOCATION: return "Memory allocation error";
        case VECTOR_ERROR_INDEX_OUT_OF_BOUNDS: return "Index out of bounds";
        case VECTOR_ERROR_EMPTY_VECTOR: return "Empty vector";
        case VECTOR_ERROR_INVALID_CAPACITY: return "Invalid capacity";
        default: return "Unknown error";
    }
}

VectorStatus create_vector(Vector *vec, size_t initial_capacity, 
                          VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE), 
                          void (*DeleteFunc)(VECTOR_TYPE)) {
    if (!vec) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    vec->capacity = initial_capacity;
    vec->size = 0;
    vec->CopyVoidPtr = CopyFunc;
    vec->DeleteVoidPtr = DeleteFunc;

    if (initial_capacity > 0) {
        vec->data = (VECTOR_TYPE*)malloc(initial_capacity * sizeof(VECTOR_TYPE));
        if (!vec->data) {
            vec->capacity = 0;
            return VECTOR_ERROR_MEMORY_ALLOCATION;
        }
    } else if (initial_capacity == 0) {
        vec->data = NULL;
    } else {
        return VECTOR_ERROR_INVALID_CAPACITY;
    }

    return VECTOR_SUCCESS;
}

VectorStatus erase_vector(Vector *v) {
    if (!v) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    if (v->data) {
        if (v->DeleteVoidPtr) {
            for (size_t i = 0; i < v->size; i++) {
                v->DeleteVoidPtr(v->data[i]);
            }
        }
        free(v->data);
        v->data = NULL;
    }
    v->size = 0;
    v->capacity = 0;
    
    return VECTOR_SUCCESS;
}

int is_equal_vector(const Vector *v1, const Vector *v2) {
    if (!v1 || !v2) {
        return 0;
    }
    if (v1->size != v2->size) {
        return 0;
    }
    
    for (size_t i = 0; i < v1->size; i++) {
        if (v1->data[i] != v2->data[i]) {
            return 0;
        }
    }
    return 1;
}

VectorStatus copy_vector(Vector *dest, const Vector *src) {
    if (!dest || !src) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    if (dest == src) {
        return VECTOR_SUCCESS;
    }
    
    VectorStatus status = erase_vector(dest);
    if (status != VECTOR_SUCCESS) {
        return status;
    }
    
    dest->size = src->size;
    dest->capacity = src->capacity;
    dest->CopyVoidPtr = src->CopyVoidPtr;
    dest->DeleteVoidPtr = src->DeleteVoidPtr;
    
    if (src->capacity > 0) {
        dest->data = (VECTOR_TYPE*)malloc(src->capacity * sizeof(VECTOR_TYPE));
        if (!dest->data) {
            dest->capacity = 0;
            dest->size = 0;
            return VECTOR_ERROR_MEMORY_ALLOCATION;
        }
        
        for (size_t i = 0; i < src->size; i++) {
            dest->data[i] = dest->CopyVoidPtr ? dest->CopyVoidPtr(src->data[i]) : src->data[i];
        }
    } else {
        dest->data = NULL;
    }
    
    return VECTOR_SUCCESS;
}

VectorStatus copy_vector_new(const Vector *src, Vector **result) {
    if (!src || !result) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    *result = (Vector*)malloc(sizeof(Vector));
    if (!*result) {
        return VECTOR_ERROR_MEMORY_ALLOCATION;
    }
    
    (*result)->size = src->size;
    (*result)->capacity = src->capacity;
    (*result)->CopyVoidPtr = src->CopyVoidPtr;
    (*result)->DeleteVoidPtr = src->DeleteVoidPtr;
    
    if (src->capacity > 0) {
        (*result)->data = (VECTOR_TYPE*)malloc(src->capacity * sizeof(VECTOR_TYPE));
        if (!(*result)->data) {
            free(*result);
            *result = NULL;
            return VECTOR_ERROR_MEMORY_ALLOCATION;
        }
        
        for (size_t i = 0; i < src->size; i++) {
            (*result)->data[i] = (*result)->CopyVoidPtr ? 
                (*result)->CopyVoidPtr(src->data[i]) : src->data[i];
        }
    } else {
        (*result)->data = NULL;
    }
    
    return VECTOR_SUCCESS;
}

VectorStatus push_back_vector(Vector *v, VECTOR_TYPE value) {
    if (!v) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    if (v->size >= v->capacity) {
        size_t new_capacity;
        if (v->capacity == 0) {
            new_capacity = 1;
        } else {
            new_capacity = v->capacity * 2;
        }
        
        VECTOR_TYPE *new_data = (VECTOR_TYPE*)realloc(v->data, new_capacity * sizeof(VECTOR_TYPE));
        if (!new_data) {
            return VECTOR_ERROR_MEMORY_ALLOCATION;
        }
        v->data = new_data;
        v->capacity = new_capacity;
    }
    
    v->data[v->size] = v->CopyVoidPtr ? v->CopyVoidPtr(value) : value;
    v->size++;
    
    return VECTOR_SUCCESS;
}

VectorStatus delete_at_vector(Vector *v, size_t index, VECTOR_TYPE *deleted_value) {
    if (!v) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    if (v->size == 0) {
        return VECTOR_ERROR_EMPTY_VECTOR;
    }
    
    if (index >= v->size) {
        return VECTOR_ERROR_INDEX_OUT_OF_BOUNDS;
    }
    
    if (deleted_value) {
        *deleted_value = v->data[index];
    }
    
    if (v->DeleteVoidPtr) {
        v->DeleteVoidPtr(v->data[index]);
    }

    for (size_t i = index; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
    
    return VECTOR_SUCCESS;
}

VectorStatus get_at_vector(const Vector *v, size_t index, VECTOR_TYPE *result) {
    if (!v || !result) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    if (index >= v->size) {
        return VECTOR_ERROR_INDEX_OUT_OF_BOUNDS;
    }
    
    *result = v->data[index];
    return VECTOR_SUCCESS;
}

VectorStatus delete_vector(Vector *v) {
    if (!v) {
        return VECTOR_ERROR_NULL_POINTER;
    }
    
    return erase_vector(v);
}
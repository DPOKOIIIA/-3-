#include <stdio.h>
#include "functions.h"

int copy_int(int value) {
    return value;
}

void delete_int(int value) {
    (void)value;
}

int main() {
    Vector vec;
    VectorStatus status;
    
    status = create_vector(&vec, 10, copy_int, delete_int);
    if (status != VECTOR_SUCCESS) {
        printf("Error creating vector: %s\n", vector_status_string(status));
        return 1;
    }
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec, i * 10);
        if (status != VECTOR_SUCCESS) {
            printf("Error pushing element: %s\n", vector_status_string(status));
            delete_vector(&vec);
            return 1;
        }
    }
    
    for (size_t i = 0; i < vec.size; i++) {
        int value;
        status = get_at_vector(&vec, i, &value);
        if (status == VECTOR_SUCCESS) {
            printf("Element at %zu: %d\n", i, value);
        }
    }
    
    Vector *vec_copy;
    status = copy_vector_new(&vec, &vec_copy);
    if (status != VECTOR_SUCCESS) {
        printf("Error copying vector: %s\n", vector_status_string(status));
        delete_vector(&vec);
        return 1;
    }
    
    printf("Vectors are equal: %d\n", is_equal_vector(&vec, vec_copy));
    
    int deleted_value;
    status = delete_at_vector(&vec, 2, &deleted_value);
    if (status == VECTOR_SUCCESS) {
        printf("Deleted value: %d\n", deleted_value);
    }

    for (size_t i = 0; i < vec.size; i++) {
        int value;
        status = get_at_vector(&vec, i, &value);
        if (status == VECTOR_SUCCESS) {
            printf("Element at %zu: %d\n", i, value);
        }
    }
    
    status = delete_vector(&vec);
    status = delete_vector(vec_copy);
    free(vec_copy);
    
    return 0;
}
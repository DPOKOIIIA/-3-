#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "functions.h"

int copy_int(int value) {
    return value;
}

void delete_int(int value) {
    (void)value;
}

int copy_int_deep(int value) {
    return value * 1;
}

void test_create_vector() {
    printf("Testing create_vector... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 10, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    assert(vec.capacity == 10);
    assert(vec.size == 0);
    assert(vec.data != NULL);
    assert(vec.CopyVoidPtr == copy_int);
    assert(vec.DeleteVoidPtr == delete_int);
    delete_vector(&vec);
    
    Vector vec2;
    status = create_vector(&vec2, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.capacity == 0);
    assert(vec2.size == 0);
    assert(vec2.data == NULL);
    delete_vector(&vec2);
    
    Vector vec3;
    status = create_vector(&vec3, 1000, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    assert(vec3.capacity == 1000);
    assert(vec3.size == 0);
    assert(vec3.data != NULL);
    delete_vector(&vec3);
    
    Vector vec4;
    status = create_vector(&vec4, 5, NULL, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec4.capacity == 5);
    assert(vec4.size == 0);
    assert(vec4.data != NULL);
    assert(vec4.CopyVoidPtr == NULL);
    assert(vec4.DeleteVoidPtr == NULL);
    delete_vector(&vec4);
    
    status = create_vector(NULL, 5, copy_int, delete_int);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    printf("PASSED\n");
}

void test_push_back_vector() {
    printf("Testing push_back_vector... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 2, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = push_back_vector(&vec, 10);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 1);
    
    int value;
    status = get_at_vector(&vec, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 10);
    
    status = push_back_vector(&vec, 20);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 2);
    
    status = get_at_vector(&vec, 1, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 20);
    
    status = push_back_vector(&vec, 30);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 3);
    assert(vec.capacity >= 3);
    
    status = get_at_vector(&vec, 2, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 30);
    
    for (int i = 0; i < 100; i++) {
        status = push_back_vector(&vec, i * 100);
        assert(status == VECTOR_SUCCESS);
    }
    assert(vec.size == 103);
    assert(vec.capacity >= 103);
    
    status = push_back_vector(NULL, 10);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    delete_vector(&vec);
    
    Vector vec2;
    status = create_vector(&vec2, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = push_back_vector(&vec2, 42);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.size == 1);
    
    status = get_at_vector(&vec2, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 42);
    
    delete_vector(&vec2);
    
    Vector vec3;
    status = create_vector(&vec3, 3, NULL, NULL);
    assert(status == VECTOR_SUCCESS);
    
    status = push_back_vector(&vec3, 123);
    assert(status == VECTOR_SUCCESS);
    assert(vec3.size == 1);
    
    status = get_at_vector(&vec3, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 123);
    
    delete_vector(&vec3);
    
    printf("PASSED\n");
}

void test_get_at_vector() {
    printf("Testing get_at_vector... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec, i * 10);
        assert(status == VECTOR_SUCCESS);
    }

    int value;
    for (int i = 0; i < 5; i++) {
        status = get_at_vector(&vec, i, &value);
        assert(status == VECTOR_SUCCESS);
        assert(value == i * 10);
    }
    
    status = get_at_vector(&vec, 10, &value);
    assert(status == VECTOR_ERROR_INDEX_OUT_OF_BOUNDS);

    status = get_at_vector(NULL, 0, &value);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    status = get_at_vector(&vec, 0, NULL);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    Vector empty;
    status = create_vector(&empty, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = get_at_vector(&empty, 0, &value);
    assert(status == VECTOR_ERROR_INDEX_OUT_OF_BOUNDS);
    
    delete_vector(&empty);
    delete_vector(&vec);
    
    printf("PASSED\n");
}

void test_delete_at_vector() {
    printf("Testing delete_at_vector... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec, i * 10);
        assert(status == VECTOR_SUCCESS);
    }
    
    int deleted_value;
    status = delete_at_vector(&vec, 2, &deleted_value);
    assert(status == VECTOR_SUCCESS);
    assert(deleted_value == 20);
    assert(vec.size == 4);
    
    int value;
    status = get_at_vector(&vec, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 0);
    
    status = get_at_vector(&vec, 1, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 10);
    
    status = get_at_vector(&vec, 2, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 30);
    
    status = get_at_vector(&vec, 3, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 40);
    
    status = delete_at_vector(&vec, 0, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 3);
    
    status = get_at_vector(&vec, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 10);
    
    status = delete_at_vector(&vec, 2, &deleted_value);
    assert(status == VECTOR_SUCCESS);
    assert(deleted_value == 40);
    assert(vec.size == 2);
    
    status = get_at_vector(&vec, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 10);
    
    status = get_at_vector(&vec, 1, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 30);
    
    status = delete_at_vector(&vec, 10, NULL);
    assert(status == VECTOR_ERROR_INDEX_OUT_OF_BOUNDS);
    assert(vec.size == 2);
    
    status = delete_at_vector(NULL, 0, NULL);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    Vector empty;
    status = create_vector(&empty, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = delete_at_vector(&empty, 0, NULL);
    assert(status == VECTOR_ERROR_EMPTY_VECTOR);
    assert(empty.size == 0);
    
    delete_vector(&empty);
    
    Vector vec2;
    status = create_vector(&vec2, 3, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&vec2, 1);
    push_back_vector(&vec2, 2);
    push_back_vector(&vec2, 3);
    
    status = delete_at_vector(&vec2, 0, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.size == 2);
    
    status = delete_at_vector(&vec2, 0, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.size == 1);
    
    status = delete_at_vector(&vec2, 0, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.size == 0);
    
    delete_vector(&vec2);
    
    Vector vec3;
    status = create_vector(&vec3, 3, copy_int, NULL);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&vec3, 1);
    push_back_vector(&vec3, 2);
    push_back_vector(&vec3, 3);
    
    status = delete_at_vector(&vec3, 1, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec3.size == 2);
    
    delete_vector(&vec3);
    
    delete_vector(&vec);
    
    printf("PASSED\n");
}

void test_copy_vector() {
    printf("Testing copy_vector... ");
    
    Vector vec1;
    VectorStatus status = create_vector(&vec1, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec1, i * 10);
        assert(status == VECTOR_SUCCESS);
    }
    
    Vector vec2;
    status = create_vector(&vec2, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = copy_vector(&vec2, &vec1);
    assert(status == VECTOR_SUCCESS);
    
    assert(vec2.size == vec1.size);
    assert(vec2.capacity == vec1.capacity);
    
    int value1, value2;
    for (size_t i = 0; i < vec1.size; i++) {
        status = get_at_vector(&vec1, i, &value1);
        assert(status == VECTOR_SUCCESS);
        status = get_at_vector(&vec2, i, &value2);
        assert(status == VECTOR_SUCCESS);
        assert(value1 == value2);
    }
    assert(vec2.data != vec1.data);
    
    Vector vec3;
    status = create_vector(&vec3, 3, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 3; i++) {
        status = push_back_vector(&vec3, i * 100);
        assert(status == VECTOR_SUCCESS);
    }
    
    status = copy_vector(&vec3, &vec1);
    assert(status == VECTOR_SUCCESS);
    assert(vec3.size == vec1.size);
    
    for (size_t i = 0; i < vec1.size; i++) {
        status = get_at_vector(&vec1, i, &value1);
        assert(status == VECTOR_SUCCESS);
        status = get_at_vector(&vec3, i, &value2);
        assert(status == VECTOR_SUCCESS);
        assert(value1 == value2);
    }
    
    status = copy_vector(NULL, &vec1);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    status = copy_vector(&vec2, NULL);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    Vector empty_src;
    status = create_vector(&empty_src, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    Vector empty_dest;
    status = create_vector(&empty_dest, 10, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = push_back_vector(&empty_dest, 42);
    assert(status == VECTOR_SUCCESS);
    
    status = copy_vector(&empty_dest, &empty_src);
    assert(status == VECTOR_SUCCESS);
    assert(empty_dest.size == 0);
    assert(empty_dest.capacity == 0);
    assert(empty_dest.data == NULL);
    
    Vector vec4;
    status = create_vector(&vec4, 3, NULL, NULL);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&vec4, 111);
    push_back_vector(&vec4, 222);
    
    Vector vec5;
    status = create_vector(&vec5, 1, NULL, NULL);
    assert(status == VECTOR_SUCCESS);
    
    status = copy_vector(&vec5, &vec4);
    assert(status == VECTOR_SUCCESS);
    assert(vec5.size == 2);
    
    status = get_at_vector(&vec5, 0, &value1);
    assert(status == VECTOR_SUCCESS);
    assert(value1 == 111);
    
    status = get_at_vector(&vec5, 1, &value1);
    assert(status == VECTOR_SUCCESS);
    assert(value1 == 222);
    
    delete_vector(&vec1);
    delete_vector(&vec2);
    delete_vector(&vec3);
    delete_vector(&empty_src);
    delete_vector(&empty_dest);
    delete_vector(&vec4);
    delete_vector(&vec5);
    
    printf("PASSED\n");
}

void test_copy_vector_new() {
    printf("Testing copy_vector_new... ");
    
    Vector vec1;
    VectorStatus status = create_vector(&vec1, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec1, i * 10);
        assert(status == VECTOR_SUCCESS);
    }
    
    Vector *vec_copy;
    status = copy_vector_new(&vec1, &vec_copy);
    assert(status == VECTOR_SUCCESS);
    assert(vec_copy != NULL);
    assert(vec_copy->size == vec1.size);
    assert(vec_copy->capacity == vec1.capacity);
    
    int value1, value2;
    for (size_t i = 0; i < vec1.size; i++) {
        status = get_at_vector(&vec1, i, &value1);
        assert(status == VECTOR_SUCCESS);
        status = get_at_vector(vec_copy, i, &value2);
        assert(status == VECTOR_SUCCESS);
        assert(value1 == value2);
    }
    assert(vec_copy->data != vec1.data);

    Vector *null_copy;
    status = copy_vector_new(NULL, &null_copy);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    status = copy_vector_new(&vec1, NULL);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    Vector empty;
    status = create_vector(&empty, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    Vector *empty_copy;
    status = copy_vector_new(&empty, &empty_copy);
    assert(status == VECTOR_SUCCESS);
    assert(empty_copy != NULL);
    assert(empty_copy->size == 0);
    assert(empty_copy->capacity == 0);
    assert(empty_copy->data == NULL);
    
    Vector vec2;
    status = create_vector(&vec2, 2, NULL, NULL);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&vec2, 999);
    push_back_vector(&vec2, 888);
    
    Vector *vec2_copy;
    status = copy_vector_new(&vec2, &vec2_copy);
    assert(status == VECTOR_SUCCESS);
    assert(vec2_copy != NULL);
    assert(vec2_copy->size == 2);
    
    status = get_at_vector(vec2_copy, 0, &value1);
    assert(status == VECTOR_SUCCESS);
    assert(value1 == 999);
    
    status = get_at_vector(vec2_copy, 1, &value1);
    assert(status == VECTOR_SUCCESS);
    assert(value1 == 888);
    
    delete_vector(&vec1);
    delete_vector(vec_copy);
    free(vec_copy);
    delete_vector(&empty);
    delete_vector(empty_copy);
    free(empty_copy);
    delete_vector(&vec2);
    delete_vector(vec2_copy);
    free(vec2_copy);
    
    printf("PASSED\n");
}

void test_is_equal_vector() {
    printf("Testing is_equal_vector... ");
    
    Vector vec1;
    VectorStatus status = create_vector(&vec1, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    Vector vec2;
    status = create_vector(&vec2, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec1, i * 10);
        assert(status == VECTOR_SUCCESS);
        status = push_back_vector(&vec2, i * 10);
        assert(status == VECTOR_SUCCESS);
    }
    
    assert(is_equal_vector(&vec1, &vec2) == 1);

    status = push_back_vector(&vec1, 50);
    assert(status == VECTOR_SUCCESS);
    assert(is_equal_vector(&vec1, &vec2) == 0);
    
    int deleted_value;
    status = delete_at_vector(&vec1, 5, &deleted_value);
    assert(status == VECTOR_SUCCESS);
    status = push_back_vector(&vec2, 60);
    assert(status == VECTOR_SUCCESS);
    assert(is_equal_vector(&vec1, &vec2) == 0);
    
    assert(is_equal_vector(NULL, &vec2) == 0);
    assert(is_equal_vector(&vec1, NULL) == 0);
    assert(is_equal_vector(NULL, NULL) == 0);
    
    Vector empty1;
    status = create_vector(&empty1, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    Vector empty2;
    status = create_vector(&empty2, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    assert(is_equal_vector(&empty1, &empty2) == 1);
    
    assert(is_equal_vector(&empty1, &vec1) == 0);
    
    Vector vec3;
    status = create_vector(&vec3, 3, copy_int_deep, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 3; i++) {
        status = push_back_vector(&vec3, i * 5);
        assert(status == VECTOR_SUCCESS);
    }
    
    Vector vec4;
    status = create_vector(&vec4, 3, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 3; i++) {
        status = push_back_vector(&vec4, i * 5);
        assert(status == VECTOR_SUCCESS);
    }
    
    assert(is_equal_vector(&vec3, &vec4) == 1);
    
    delete_vector(&vec1);
    delete_vector(&vec2);
    delete_vector(&empty1);
    delete_vector(&empty2);
    delete_vector(&vec3);
    delete_vector(&vec4);
    
    printf("PASSED\n");
}

void test_erase_vector() {
    printf("Testing erase_vector... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec, i * 10);
        assert(status == VECTOR_SUCCESS);
    }
    
    status = erase_vector(&vec);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    
    status = erase_vector(&vec);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    
    status = erase_vector(NULL);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    Vector empty;
    status = create_vector(&empty, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = erase_vector(&empty);
    assert(status == VECTOR_SUCCESS);
    assert(empty.size == 0);
    assert(empty.capacity == 0);
    assert(empty.data == NULL);
    
    delete_vector(&empty);
    
    Vector vec2;
    status = create_vector(&vec2, 3, copy_int, NULL);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&vec2, 1);
    push_back_vector(&vec2, 2);
    push_back_vector(&vec2, 3);
    
    status = erase_vector(&vec2);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.size == 0);
    assert(vec2.capacity == 0);
    assert(vec2.data == NULL);
    
    delete_vector(&vec2);
    
    printf("PASSED\n");
}

void test_delete_vector() {
    printf("Testing delete_vector... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 5; i++) {
        status = push_back_vector(&vec, i * 10);
        assert(status == VECTOR_SUCCESS);
    }
    
    status = delete_vector(&vec);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    
    status = delete_vector(&vec);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    
    status = delete_vector(NULL);
    assert(status == VECTOR_ERROR_NULL_POINTER);
    
    Vector empty;
    status = create_vector(&empty, 0, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = delete_vector(&empty);
    assert(status == VECTOR_SUCCESS);
    assert(empty.size == 0);
    assert(empty.capacity == 0);
    assert(empty.data == NULL);
    
    Vector vec2;
    status = create_vector(&vec2, 2, NULL, NULL);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&vec2, 100);
    push_back_vector(&vec2, 200);
    
    status = delete_vector(&vec2);
    assert(status == VECTOR_SUCCESS);
    assert(vec2.size == 0);
    assert(vec2.capacity == 0);
    assert(vec2.data == NULL);
    
    printf("PASSED\n");
}

void test_edge_cases() {
    printf("Testing edge cases... ");
    
    Vector large;
    VectorStatus status = create_vector(&large, 1, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int i = 0; i < 10000; i++) {
        status = push_back_vector(&large, i);
        assert(status == VECTOR_SUCCESS);
    }
    assert(large.size == 10000);
    assert(large.capacity >= 10000);
    
    int value;
    status = get_at_vector(&large, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 0);
    
    status = get_at_vector(&large, 9999, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 9999);
    
    status = get_at_vector(&large, 5000, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 5000);
    
    delete_vector(&large);
    
    Vector vec;
    status = create_vector(&vec, 1, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    for (int cycle = 0; cycle < 100; cycle++) {
        for (int i = 0; i < 10; i++) {
            status = push_back_vector(&vec, i);
            assert(status == VECTOR_SUCCESS);
        }
        assert(vec.size == 10);
        
        for (int i = 0; i < 10; i++) {
            status = delete_at_vector(&vec, 0, NULL);
            assert(status == VECTOR_SUCCESS);
        }
        assert(vec.size == 0);
    }
    delete_vector(&vec);
    
    Vector self;
    status = create_vector(&self, 3, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&self, 1);
    push_back_vector(&self, 2);
    push_back_vector(&self, 3);
    
    status = copy_vector(&self, &self);
    assert(status == VECTOR_SUCCESS);
    assert(self.size == 3);
    
    status = get_at_vector(&self, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 1);
    
    status = get_at_vector(&self, 1, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 2);
    
    status = get_at_vector(&self, 2, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 3);
    
    delete_vector(&self);
    
    Vector original;
    status = create_vector(&original, 3, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    push_back_vector(&original, 10);
    push_back_vector(&original, 20);
    push_back_vector(&original, 30);
    
    Vector *copy1;
    status = copy_vector_new(&original, &copy1);
    assert(status == VECTOR_SUCCESS);
    
    Vector *copy2;
    status = copy_vector_new(copy1, &copy2);
    assert(status == VECTOR_SUCCESS);
    
    Vector *copy3;
    status = copy_vector_new(copy2, &copy3);
    assert(status == VECTOR_SUCCESS);
    
    assert(is_equal_vector(&original, copy1));
    assert(is_equal_vector(copy1, copy2));
    assert(is_equal_vector(copy2, copy3));
    
    delete_vector(&original);
    delete_vector(copy1);
    free(copy1);
    delete_vector(copy2);
    free(copy2);
    delete_vector(copy3);
    free(copy3);
    
    printf("PASSED\n");
}

void test_zero_and_negative() {
    printf("Testing zero and negative values... ");
    
    Vector vec;
    VectorStatus status = create_vector(&vec, 5, copy_int, delete_int);
    assert(status == VECTOR_SUCCESS);
    
    status = push_back_vector(&vec, 0);
    assert(status == VECTOR_SUCCESS);
    status = push_back_vector(&vec, 0);
    assert(status == VECTOR_SUCCESS);
    status = push_back_vector(&vec, 0);
    assert(status == VECTOR_SUCCESS);
    
    assert(vec.size == 3);
    
    int value;
    status = get_at_vector(&vec, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 0);
    
    status = get_at_vector(&vec, 1, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 0);
    
    status = get_at_vector(&vec, 2, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 0);
    
    status = push_back_vector(&vec, -10);
    assert(status == VECTOR_SUCCESS);
    status = push_back_vector(&vec, -20);
    assert(status == VECTOR_SUCCESS);
    status = push_back_vector(&vec, -30);
    assert(status == VECTOR_SUCCESS);
    
    assert(vec.size == 6);
    
    status = get_at_vector(&vec, 3, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == -10);
    
    status = get_at_vector(&vec, 4, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == -20);
    
    status = get_at_vector(&vec, 5, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == -30);
    
    status = delete_at_vector(&vec, 0, NULL);
    assert(status == VECTOR_SUCCESS);
    assert(vec.size == 5);
    
    status = get_at_vector(&vec, 0, &value);
    assert(status == VECTOR_SUCCESS);
    assert(value == 0);
    
    delete_vector(&vec);
    
    printf("PASSED\n");
}

void run_all_tests() {
    printf("Running comprehensive vector tests...\n\n");
    
    test_create_vector();
    test_push_back_vector();
    test_get_at_vector();
    test_delete_at_vector();
    test_copy_vector();
    test_copy_vector_new();
    test_is_equal_vector();
    test_erase_vector();
    test_delete_vector();
    test_edge_cases();
    test_zero_and_negative();
    
    printf("\nAll tests passed!\n");
}

int main() {
    run_all_tests();
    return 0;
}
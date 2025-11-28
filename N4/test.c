#include "functions.h"
#include <assert.h>

void test_heap_operations();
void test_post_system();
void test_error_handling();

int int_cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int main() {
    test_heap_operations();
    test_post_system();
    test_error_handling();
    printf("All tests passed!\n");
    return 0;
}

void test_heap_operations() {
    printf("Testing heap operations...\n");
    
    Heap h;
    StatusCode status = heap_create(&h, 10, int_cmp);
    assert(status == SUCCESS);
    
    int empty;
    status = heap_is_empty(&h, &empty);
    assert(status == SUCCESS && empty == 1);
    
    size_t size;
    status = heap_size(&h, &size);
    assert(status == SUCCESS && size == 0);

    int values[] = {5, 3, 8, 1, 9};
    for (int i = 0; i < 5; i++) {
        int *val = checked_malloc(sizeof(int));
        *val = values[i];
        status = heap_push(&h, val);
        assert(status == SUCCESS);
    }
    
    status = heap_is_empty(&h, &empty);
    assert(status == SUCCESS && empty == 0);
    
    status = heap_size(&h, &size);
    assert(status == SUCCESS && size == 5);
    
    void *min_ptr;
    status = heap_peek(&h, &min_ptr);
    assert(status == SUCCESS && *(int*)min_ptr == 1);

    status = heap_pop(&h, &min_ptr);
    assert(status == SUCCESS && *(int*)min_ptr == 1);
    free(min_ptr);
    
    status = heap_pop(&h, &min_ptr);
    assert(status == SUCCESS && *(int*)min_ptr == 3);
    free(min_ptr);
    
    status = heap_size(&h, &size);
    assert(status == SUCCESS && size == 3);

    while (!empty) {
        status = heap_pop(&h, &min_ptr);
        if (status == SUCCESS) free(min_ptr);
        status = heap_is_empty(&h, &empty);
    }
    
    status = heap_delete(&h);
    assert(status == SUCCESS);
    
    printf("Heap operations tests passed!\n");
}

void test_post_system() {
    printf("Testing postal system...\n");
    
    PostSystem system;
    StatusCode status = post_system_create(&system, "test.log");
    assert(status == SUCCESS);

    int neighbors1[] = {2};
    int neighbors2[] = {1};
    
    status = post_office_add(&system, 1, (size_t)3, neighbors1, 1);
    assert(status == SUCCESS);
    
    status = post_office_add(&system, 2, (size_t)3, neighbors2, 1);
    assert(status == SUCCESS);
    
    int office_idx = find_office_index(&system, 1);
    assert(office_idx == 0);
    
    office_idx = find_office_index(&system, 2);
    assert(office_idx == 1);

    status = post_office_add(&system, 1, (size_t)3, neighbors1, 1);
    assert(status == ERROR_INVALID_OFFICE_ID);

    int letter_id;
    status = letter_add(&system, "ordinary", 1, 1, 2, "test", &letter_id);
    assert(status == SUCCESS);
    assert(system.letters_count == 1);
    assert(system.letters[0].id == 1);
    assert(system.letters[0].current_office_id == 1);

    status = letter_add(&system, "urgent", 5, 2, 1, "test2", &letter_id);
    assert(status == SUCCESS);
    assert(system.letters_count == 2);

    status = letter_mark_undelivered(&system, 1);
    assert(status == SUCCESS);
    assert(system.letters[0].state == 2);

    status = letters_process_delivery(&system);
    assert(status == SUCCESS);

    int success;
    status = letter_try_take(&system, 2, 1, &success);
    assert(status == SUCCESS);

    status = post_office_remove(&system, 1);
    assert(status == SUCCESS);
    assert(find_office_index(&system, 1) == -1);

    status = letters_print_all(&system, "test_output.txt");
    assert(status == SUCCESS);
    
    status = post_system_delete(&system);
    assert(status == SUCCESS);
    
    printf("Postal system tests passed!\n");
}

void test_error_handling() {
    printf("Testing error handling...\n");

    StatusCode status = heap_create(NULL, 10, int_cmp);
    assert(status == ERROR_NULL_POINTER);
    
    status = heap_is_empty(NULL, NULL);
    assert(status == ERROR_NULL_POINTER);
    
    Heap h;
    status = heap_peek(NULL, NULL);
    assert(status == ERROR_NULL_POINTER);

    status = heap_create(&h, 0, int_cmp);
    assert(status == ERROR_INVALID_PARAMETER);
    
    PostSystem system;
    status = post_system_create(NULL, "test.log");
    assert(status == ERROR_NULL_POINTER);
    
    status = post_system_create(&system, NULL);
    assert(status == ERROR_NULL_POINTER);

    status = heap_create(&h, 5, int_cmp);
    assert(status == SUCCESS);
    
    void *result;
    status = heap_pop(&h, &result);
    assert(status == ERROR_EMPTY_HEAP);
    
    status = heap_peek(&h, &result);
    assert(status == ERROR_EMPTY_HEAP);
    
    status = heap_delete(&h);
    assert(status == SUCCESS);

    status = post_system_create(&system, "test_error.log");
    assert(status == SUCCESS);

    int neighbors[] = {2};
    status = post_office_add(&system, -1, (size_t)5, neighbors, 1);
    assert(status == ERROR_INVALID_PARAMETER);
    
    status = post_office_add(&system, 1, (size_t)0, neighbors, 1);
    assert(status == ERROR_INVALID_PARAMETER);

    status = post_office_remove(&system, 999);
    assert(status == ERROR_NOT_FOUND);
    
    int letter_id;
    status = letter_add(&system, "test", 1, 999, 1, "data", &letter_id);
    assert(status == ERROR_NOT_FOUND);
    
    status = letter_add(&system, "test", 1, 1, 999, "data", &letter_id);
    assert(status == ERROR_NOT_FOUND);
    
    status = letter_mark_undelivered(&system, 999);
    assert(status == ERROR_NOT_FOUND);
    
    status = post_system_delete(&system);
    assert(status == SUCCESS);
    
    printf("Error handling tests passed!\n");
}
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER,
    ERROR_INVALID_PARAMETER,
    ERROR_MEMORY_ALLOCATION,
    ERROR_NOT_FOUND,
    ERROR_CAPACITY_EXCEEDED,
    ERROR_EMPTY_HEAP,
    ERROR_FILE_OPERATION,
    ERROR_INVALID_OFFICE_ID
} StatusCode;

typedef struct {
    void **data;
    size_t size;
    size_t capacity;
    int (*cmp)(const void*, const void*);
} Heap;

typedef struct {
    int id;
    char type[20];
    int state;
    int priority;
    int from_office_id;
    int to_office_id;
    char technical_data[100];
    int current_office_id;
    int *visited_offices;
    int visited_count;
    int visited_capacity;
} Letter;

typedef struct {
    int id;
    size_t max_letters;
    Heap letters_heap;
    int *neighbors;
    int neighbors_count;
} PostOffice;

typedef struct {
    PostOffice *offices;
    int offices_count;
    int offices_capacity;
    Letter *letters;
    int letters_count;
    int letters_capacity;
    int next_letter_id;
    FILE *log_file;
} PostSystem;

StatusCode heap_create(Heap *heap, size_t initial_capacity, int (*cmp)(const void*, const void*));
StatusCode heap_delete(Heap *heap);
StatusCode heap_is_empty(const Heap *heap, int *result);
StatusCode heap_size(const Heap *heap, size_t *result);
StatusCode heap_peek(const Heap *heap, void **result);
StatusCode heap_push(Heap *heap, void *value);
StatusCode heap_pop(Heap *heap, void **result);
StatusCode heap_build(Heap *heap, void **array, size_t n, int (*cmp)(const void*, const void*));
StatusCode heap_is_equal(const Heap *h1, const Heap *h2, int (*cmp)(const void*, const void*), int *result);
StatusCode post_system_create(PostSystem *system, const char *log_filename);
StatusCode post_system_delete(PostSystem *system);
StatusCode post_office_add(PostSystem *system, int id, size_t max_letters, const int *neighbors, int neighbors_count);
StatusCode post_office_remove(PostSystem *system, int id);
StatusCode letter_add(PostSystem *system, const char *type, int priority, int from_office, int to_office, const char *tech_data, int *letter_id);
StatusCode letter_mark_undelivered(PostSystem *system, int letter_id);
StatusCode letter_try_take(PostSystem *system, int letter_id, int office_id, int *success);
StatusCode letters_process_delivery(PostSystem *system);
StatusCode letters_print_all(const PostSystem *system, const char *filename);

int letter_cmp(const void *a, const void *b);
void* checked_malloc(size_t size);
void* checked_realloc(void *ptr, size_t size);
int find_office_index(const PostSystem *system, int id);

#endif
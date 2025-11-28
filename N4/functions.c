#include "functions.h"

static StatusCode heapify_up(Heap *h, size_t index);
static StatusCode heapify_down(Heap *h, size_t index);
static void swap(void **a, void **b);

void* checked_malloc(size_t size) {
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    return ptr;
}

void* checked_realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    void *new_ptr = realloc(ptr, size);
    return new_ptr;
}

StatusCode heap_create(Heap *heap, size_t initial_capacity, int (*cmp)(const void*, const void*)) {
    if (heap == NULL || cmp == NULL) {
        return ERROR_NULL_POINTER;
    }
    if (initial_capacity == 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    heap->data = checked_malloc(initial_capacity * sizeof(void*));
    if (heap->data == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->cmp = cmp;
    return SUCCESS;
}

StatusCode heap_delete(Heap *heap) {
    if (heap == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
    heap->cmp = NULL;
    return SUCCESS;
}

StatusCode heap_is_empty(const Heap *heap, int *result) {
    if (heap == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    *result = (heap->size == 0);
    return SUCCESS;
}

StatusCode heap_size(const Heap *heap, size_t *result) {
    if (heap == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    *result = heap->size;
    return SUCCESS;
}

StatusCode heap_peek(const Heap *heap, void **result) {
    if (heap == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    int empty;
    StatusCode status = heap_is_empty(heap, &empty);
    if (status != SUCCESS) {
        return status;
    }
    
    if (empty) {
        return ERROR_EMPTY_HEAP;
    }
    
    *result = heap->data[0];
    return SUCCESS;
}

StatusCode heap_push(Heap *heap, void *value) {
    if (heap == NULL || value == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (heap->size >= heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        if (new_capacity == 0) new_capacity = 1;
        
        void **new_data = checked_realloc(heap->data, new_capacity * sizeof(void*));
        if (new_data == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        heap->data = new_data;
        heap->capacity = new_capacity;
    }
    
    heap->data[heap->size] = value;
    heap->size++;
    
    return heapify_up(heap, heap->size - 1);
}

StatusCode heap_pop(Heap *heap, void **result) {
    if (heap == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    int empty;
    StatusCode status = heap_is_empty(heap, &empty);
    if (status != SUCCESS) {
        return status;
    }
    
    if (empty) {
        return ERROR_EMPTY_HEAP;
    }
    
    *result = heap->data[0];
    heap->size--;
    
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        status = heapify_down(heap, 0);
        if (status != SUCCESS) {
            return status;
        }
    }
    
    return SUCCESS;
}

StatusCode heap_build(Heap *heap, void **array, size_t n, int (*cmp)(const void*, const void*)) {
    if (heap == NULL || array == NULL || cmp == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (heap->capacity < n) {
        void **new_data = checked_realloc(heap->data, n * sizeof(void*));
        if (new_data == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        heap->data = new_data;
        heap->capacity = n;
    }
    
    memcpy(heap->data, array, n * sizeof(void*));
    heap->size = n;
    heap->cmp = cmp;
    
    for (int i = n / 2 - 1; i >= 0; i--) {
        StatusCode status = heapify_down(heap, i);
        if (status != SUCCESS) {
            return status;
        }
    }
    
    return SUCCESS;
}

StatusCode heap_is_equal(const Heap *h1, const Heap *h2, int (*cmp)(const void*, const void*), int *result) {
    if (h1 == NULL || h2 == NULL || cmp == NULL || result == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (h1->size != h2->size) {
        *result = 0;
        return SUCCESS;
    }
    
    for (size_t i = 0; i < h1->size; i++) {
        if (cmp(h1->data[i], h2->data[i]) != 0) {
            *result = 0;
            return SUCCESS;
        }
    }
    
    *result = 1;
    return SUCCESS;
}

StatusCode heap_remove(Heap *heap, void *value, int (*cmp)(const void*, const void*)) {
    if (heap == NULL || value == NULL || cmp == NULL) {
        return ERROR_NULL_POINTER;
    }

    size_t index = heap->size;
    for (size_t i = 0; i < heap->size; i++) {
        if (cmp(heap->data[i], value) == 0) {
            index = i;
            break;
        }
    }

    if (index == heap->size) {
        return ERROR_NOT_FOUND;
    }

    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;

    if (index < heap->size) {
        StatusCode status = heapify_down(heap, index);
        if (status != SUCCESS) return status;
        status = heapify_up(heap, index);
        if (status != SUCCESS) return status;
    }

    return SUCCESS;
}

static StatusCode heapify_up(Heap *h, size_t index) {
    if (h == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (h->cmp(h->data[index], h->data[parent]) < 0) {
            swap(&h->data[index], &h->data[parent]);
            index = parent;
        } else {
            break;
        }
    }
    return SUCCESS;
}

static StatusCode heapify_down(Heap *h, size_t index) {
    if (h == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    size_t left, right, smallest;
    
    while (index < h->size) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;
        
        if (left < h->size && h->cmp(h->data[left], h->data[smallest]) < 0) {
            smallest = left;
        }
        
        if (right < h->size && h->cmp(h->data[right], h->data[smallest]) < 0) {
            smallest = right;
        }
        
        if (smallest != index) {
            swap(&h->data[index], &h->data[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
    return SUCCESS;
}

static void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

int letter_cmp(const void *a, const void *b) {
    const Letter *la = (const Letter*)a;
    const Letter *lb = (const Letter*)b;
    return lb->priority - la->priority;
}

StatusCode post_system_create(PostSystem *system, const char *log_filename) {
    if (system == NULL || log_filename == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    system->offices_capacity = 10;
    system->offices = checked_malloc(system->offices_capacity * sizeof(PostOffice));
    if (system->offices == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    system->letters_capacity = 100;
    system->letters = checked_malloc(system->letters_capacity * sizeof(Letter));
    if (system->letters == NULL) {
        free(system->offices);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    system->offices_count = 0;
    system->letters_count = 0;
    system->next_letter_id = 1;
    
    system->log_file = fopen(log_filename, "w");
    if (system->log_file == NULL) {
        free(system->offices);
        free(system->letters);
        return ERROR_FILE_OPERATION;
    }
    
    fprintf(system->log_file, "Post system initialized\n");
    fflush(system->log_file);
    
    return SUCCESS;
}

StatusCode post_system_delete(PostSystem *system) {
    if (system == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    for (int i = 0; i < system->offices_count; i++) {
        heap_delete(&system->offices[i].letters_heap);
        free(system->offices[i].neighbors);
    }
    free(system->offices);
    
    for (int i = 0; i < system->letters_count; i++) {
        free(system->letters[i].visited_offices);
    }
    free(system->letters);
    
    if (system->log_file) {
        fprintf(system->log_file, "Post system shutdown\n");
        fclose(system->log_file);
    }
    
    return SUCCESS;
}

int find_office_index(const PostSystem *system, int id) {
    if (system == NULL) return -1;
    
    for (int i = 0; i < system->offices_count; i++) {
        if (system->offices[i].id == id) return i;
    }
    return -1;
}

StatusCode post_office_add(PostSystem *system, int id, size_t max_letters, const int *neighbors, int neighbors_count) {
    if (system == NULL || neighbors == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (id < 0 || max_letters == 0 || neighbors_count < 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    if (find_office_index(system, id) != -1) {
        return ERROR_INVALID_OFFICE_ID;
    }
    
    if (system->offices_count >= system->offices_capacity) {
        system->offices_capacity *= 2;
        PostOffice *new_offices = checked_realloc(system->offices, system->offices_capacity * sizeof(PostOffice));
        if (new_offices == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        system->offices = new_offices;
    }
    
    PostOffice *office = &system->offices[system->offices_count];
    office->id = id;
    office->max_letters = max_letters;
    office->neighbors_count = neighbors_count;
    
    if (neighbors_count > 0) {
        office->neighbors = checked_malloc(neighbors_count * sizeof(int));
        if (office->neighbors == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        memcpy(office->neighbors, neighbors, neighbors_count * sizeof(int));
    } else {
        office->neighbors = NULL;
    }
    
    StatusCode status = heap_create(&office->letters_heap, 10, letter_cmp);
    if (status != SUCCESS) {
        free(office->neighbors);
        return status;
    }
    
    system->offices_count++;
    
    if (system->log_file) {
        fprintf(system->log_file, "Added post office %d with capacity %zu\n", id, max_letters);
        fflush(system->log_file);
    }
    
    return SUCCESS;
}

StatusCode post_office_remove(PostSystem *system, int id) {
    if (system == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    int office_idx = find_office_index(system, id);
    if (office_idx == -1) {
        return ERROR_NOT_FOUND;
    }
    
    PostOffice *office = &system->offices[office_idx];
    
    while (1) {
        int empty;
        StatusCode status = heap_is_empty(&office->letters_heap, &empty);
        if (status != SUCCESS) return status;
        
        if (empty) break;
        
        void *letter_ptr;
        status = heap_pop(&office->letters_heap, &letter_ptr);
        if (status != SUCCESS) return status;
        
        Letter *letter = (Letter*)letter_ptr;
        
        if (letter->to_office_id == id || letter->from_office_id == id) {
            status = letter_mark_undelivered(system, letter->id);
            if (status != SUCCESS) return status;
        } else {
            int delivered = 0;
            for (int i = 0; i < system->offices_count && !delivered; i++) {
                if (i != office_idx && system->offices[i].id != id) {
                    size_t current_size;
                    status = heap_size(&system->offices[i].letters_heap, &current_size);
                    if (status != SUCCESS) return status;
                    
                    if (current_size < system->offices[i].max_letters) {
                        status = heap_push(&system->offices[i].letters_heap, letter);
                        if (status != SUCCESS) return status;
                        
                        letter->current_office_id = system->offices[i].id;
                        delivered = 1;
                        
                        if (system->log_file) {
                            fprintf(system->log_file, "Letter %d redirected to office %d\n", 
                                   letter->id, system->offices[i].id);
                            fflush(system->log_file);
                        }
                    }
                }
            }
            
            if (!delivered) {
                status = letter_mark_undelivered(system, letter->id);
                if (status != SUCCESS) return status;
            }
        }
    }
    
    heap_delete(&office->letters_heap);
    free(office->neighbors);
    
    for (int i = office_idx; i < system->offices_count - 1; i++) {
        system->offices[i] = system->offices[i + 1];
    }
    system->offices_count--;
    
    if (system->log_file) {
        fprintf(system->log_file, "Removed post office %d\n", id);
        fflush(system->log_file);
    }
    
    return SUCCESS;
}

StatusCode letter_add(PostSystem *system, const char *type, int priority, int from_office, int to_office, const char *tech_data, int *letter_id) {
    if (system == NULL || type == NULL || tech_data == NULL || letter_id == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (strlen(type) == 0 || strlen(tech_data) == 0 || priority < 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    if (find_office_index(system, from_office) == -1 || find_office_index(system, to_office) == -1) {
        return ERROR_NOT_FOUND;
    }
    
    if (system->letters_count >= system->letters_capacity) {
        system->letters_capacity *= 2;
        Letter *new_letters = checked_realloc(system->letters, system->letters_capacity * sizeof(Letter));
        if (new_letters == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        system->letters = new_letters;
    }
    
    Letter *letter = &system->letters[system->letters_count];
    letter->id = system->next_letter_id++;
    strncpy(letter->type, type, sizeof(letter->type) - 1);
    letter->type[sizeof(letter->type) - 1] = '\0';
    letter->priority = priority;
    letter->from_office_id = from_office;
    letter->to_office_id = to_office;
    strncpy(letter->technical_data, tech_data, sizeof(letter->technical_data) - 1);
    letter->technical_data[sizeof(letter->technical_data) - 1] = '\0';
    letter->state = 0;
    letter->current_office_id = from_office;
    letter->visited_count = 0;
    letter->visited_capacity = 10;
    letter->visited_offices = checked_malloc(letter->visited_capacity * sizeof(int));
    if (letter->visited_offices == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    letter->visited_offices[letter->visited_count++] = from_office;
    
    int office_idx = find_office_index(system, from_office);
    if (office_idx != -1) {
        PostOffice *office = &system->offices[office_idx];
        size_t current_size;
        StatusCode status = heap_size(&office->letters_heap, &current_size);
        if (status != SUCCESS) {
            free(letter->visited_offices);
            return status;
        }
        
        if (current_size < office->max_letters) {
            status = heap_push(&office->letters_heap, letter);
            if (status != SUCCESS) {
                free(letter->visited_offices);
                return status;
            }
            
            system->letters_count++;
            *letter_id = letter->id;
            
            if (system->log_file) {
                fprintf(system->log_file, "Added letter %d from office %d to office %d\n", 
                       letter->id, from_office, to_office);
                fflush(system->log_file);
            }
            return SUCCESS;
        }
    }
    
    free(letter->visited_offices);
    return ERROR_CAPACITY_EXCEEDED;
}

StatusCode letter_mark_undelivered(PostSystem *system, int letter_id) {
    if (system == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    for (int i = 0; i < system->letters_count; i++) {
        if (system->letters[i].id == letter_id) {
            system->letters[i].state = 2;
            
            if (system->log_file) {
                fprintf(system->log_file, "Marked letter %d as undelivered\n", letter_id);
                fflush(system->log_file);
            }
            return SUCCESS;
        }
    }
    
    return ERROR_NOT_FOUND;
}

StatusCode letter_try_take(PostSystem *system, int letter_id, int office_id, int *success) {
    if (system == NULL || success == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    *success = 0;
    
    for (int i = 0; i < system->letters_count; i++) {
        if (system->letters[i].id == letter_id && 
            system->letters[i].to_office_id == office_id &&
            system->letters[i].current_office_id == office_id &&
            system->letters[i].state == 0) {
            
            system->letters[i].state = 1;

            int office_idx = find_office_index(system, office_id);
            if (office_idx != -1) {
                Heap *heap = &system->offices[office_idx].letters_heap;
                StatusCode status = heap_remove(heap, &system->letters[i], letter_cmp);
                if (status != SUCCESS) return status;
            }
            
            *success = 1;
            
            if (system->log_file) {
                fprintf(system->log_file, "Letter %d delivered at office %d\n", letter_id, office_id);
                fflush(system->log_file);
            }
            return SUCCESS;
        }
    }
    
    return SUCCESS;
}

StatusCode letters_process_delivery(PostSystem *system) {
    if (system == NULL) {
        return ERROR_NULL_POINTER;
    }

    for (int i = 0; i < system->letters_count; i++) {
        Letter *letter = &system->letters[i];
        if (letter->state == 0 && letter->current_office_id == letter->to_office_id) {
            int success;
            StatusCode status = letter_try_take(system, letter->id, letter->to_office_id, &success);
            if (status != SUCCESS) return status;
        }
    }

    int letters_moved_this_cycle = 0;
    
    for (int i = 0; i < system->letters_count; i++) {
        Letter *letter = &system->letters[i];
        if (letter->state != 0) continue;
        if (letter->current_office_id == letter->to_office_id) continue;
        
        int current_office_idx = find_office_index(system, letter->current_office_id);
        if (current_office_idx == -1) {
            letter_mark_undelivered(system, letter->id);
            continue;
        }
        
        PostOffice *current_office = &system->offices[current_office_idx];
        int moved = 0;

        for (int j = 0; j < current_office->neighbors_count && !moved; j++) {
            int neighbor_id = current_office->neighbors[j];
            int neighbor_idx = find_office_index(system, neighbor_id);
            if (neighbor_idx == -1) continue;

            int already_visited = 0;
            for (int k = 0; k < letter->visited_count; k++) {
                if (letter->visited_offices[k] == neighbor_id) {
                    already_visited = 1;
                    break;
                }
            }
            if (already_visited) continue;
            
            PostOffice *neighbor = &system->offices[neighbor_idx];
            size_t neighbor_size;
            StatusCode status = heap_size(&neighbor->letters_heap, &neighbor_size);
            if (status != SUCCESS) continue;

            if (neighbor_size < neighbor->max_letters) {
                status = heap_remove(&current_office->letters_heap, letter, letter_cmp);
                if (status != SUCCESS) continue;

                status = heap_push(&neighbor->letters_heap, letter);
                if (status != SUCCESS) continue;
                
                letter->current_office_id = neighbor_id;

                if (letter->visited_count >= letter->visited_capacity) {
                    letter->visited_capacity *= 2;
                    int *new_visited = checked_realloc(letter->visited_offices, 
                                                     letter->visited_capacity * sizeof(int));
                    if (new_visited == NULL) continue;
                    letter->visited_offices = new_visited;
                }
                letter->visited_offices[letter->visited_count++] = neighbor_id;
                
                moved = 1;
                letters_moved_this_cycle++;
                
                if (system->log_file) {
                    fprintf(system->log_file, 
                           "Letter %d moved from office %d to office %d\n",
                           letter->id, current_office->id, neighbor_id);
                    fflush(system->log_file);
                }

                if (letters_moved_this_cycle >= 2) {
                    break;
                }
            }
        }
        
        if (!moved) {
            if (letter->visited_count > 8) {
                letter_mark_undelivered(system, letter->id);
            }
        }

        if (letters_moved_this_cycle >= 2) {
            break;
        }
    }
    
    return SUCCESS;
}

StatusCode letters_print_all(const PostSystem *system, const char *filename) {
    if (system == NULL || filename == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    FILE *output = fopen(filename, "w");
    if (output == NULL) {
        return ERROR_FILE_OPERATION;
    }
    
    fprintf(output, "All letters in the system:\n");
    fprintf(output, "ID\tType\tPriority\tFrom\tTo\tState\tCurrent Office\n");
    
    for (int i = 0; i < system->letters_count; i++) {
        const Letter *l = &system->letters[i];
        const char *state_str = "In transit";
        if (l->state == 1) state_str = "Delivered";
        else if (l->state == 2) state_str = "Undelivered";
        
        fprintf(output, "%d\t%s\t%d\t%d\t%d\t%s\t%d\n",
               l->id, l->type, l->priority, l->from_office_id, 
               l->to_office_id, state_str, l->current_office_id);
    }
    
    fclose(output);
    return SUCCESS;
}
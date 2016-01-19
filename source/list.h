/*
 * Thread-safe list/vector implementation
 *
 * By: Khile Dupuis
 *
 */
#ifndef LIST_H
#define LIST_H

#define LIST_MIN_SIZE 1

/*
 * Error Values
 *
 * Note: Obtaining an error value from any function means that all future
 *          operations on the list are undefined.
 *
 * SUCCESS: 0 is returned when there are no errors
 * NORELEASE: Cannot release list mutex
 * NOAQUIRE: Cannot aquire list mutex
 * HELD: Non-blocking lock failed because mutex is already held
 * NODESTROY: Cannot destoy list mutex
 * NOINIT: Cannot init list mutex
 * NOALLOCATE: Cannot allocate/deallocate memory
 * LISTEMPTY: List is empty
 *
 */
typedef enum {
    SUCCESS,
    NORELEASE,
    NOAQUIRE,
    HELD,
    NODESTROY,
    NOINIT,
    NOALLOCATE,
    LISTEMPTY
} list_result;

typedef struct {
    pthread_mutex_t lock;   /* Mutex used to make lists thread safe */
    size_t size;            /* Size of data */
    size_t length;          /* Number of items in data */
    void** data;
} List;

list_result init_list_size(List* list, size_t size);
list_result init_list(List* list);
list_result destroy_list(List* list);
list_result list_append(List* list, void* item);
void* list_pop(List* list, list_result* error);
list_result list_remove(List* list, size_t index);

#endif /* LIST_H */

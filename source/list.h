/*
 * Thread-safe list/vector implementation
 *
 * By: Khile Dupuis
 *
 */
#ifndef LIST_H
#define LIST_H


#define DEBUG 1
#define VERBOSE 1

#define MIN_SIZE 1

/*
 * Error values returned from functions returning int.
 *
 * NORELEASE: Cannot release list mutex
 * NOAQUIRE: Cannot aquire list mutex
 * HELD: Non-blocking lock failed because mutex is already held
 * NODESTROY: Cannot destoy list mutex
 * NOINIT: Cannot init list mutex
 * NOALLOCATE: Cannot allocate/deallocate memory
 * LISTEMPTY: List is empty
 * UNKOWN: Undefined error
 *
 */
#define NORELEASE 1
#define NOAQUIRE 2
#define HELD 3
#define NODESTROY 4
#define NOINIT 5
#define NOALLOCATE 6
#define LISTEMPTY 7
#define UNKOWN 8


typedef struct {
    pthread_mutex_t lock;   /* Mutex used to make lists thread safe */
    size_t size;            /* Size of data */
    size_t length;          /* Number of items in data */
    void** data;
} List;


int init_list_size(List* list, size_t size);
int init_list(List* list);
int destroy_list(List* list);

int list_append(List* list, void* item);
void* list_pop(List* list);
int list_remove(List* list, size_t index);


#endif /* LIST_H */

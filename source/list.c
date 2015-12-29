/*
 * Thread-safe list/vector implementation
 *
 * By: Khile Dupuis
 *
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "list.h"

int init_list_size(List* list, size_t size)
{
    int result;

    result = pthread_mutex_init(&list->lock, NULL);
    if (result)
        return NOINIT; /* Can not init mutex */
    list->data = (void**)malloc(size * sizeof(void*));
    if (list->data == NULL) {
        perror("new_list_size");
        return NOALLOCATE; /* Memory allocation failed */
    }
    list->size = size;
    list->length = 0;
    return 0;
}

int init_list(List* list)
{
    return init_list_size(list, MIN_SIZE);
}

int destroy_list(List* list)
{
    int result;

    result = pthread_mutex_trylock(&list->lock);
    if (result == EBUSY) {
        return HELD; /* The lock is currently being held */
    } else if (result) {
        return NOAQUIRE; /* Cannot aquire the lock */
    }
    free(list->data);

    result = pthread_mutex_unlock(&list->lock);
    if (result)
        return NORELEASE; /* Cannot release lock */
    result = pthread_mutex_destroy(&list->lock);
    if (result)
        return NODESTROY; /* Lock cannot be destroyed */

    return 0;
}

int list_lock(List* list)
{
    int result;
    result = pthread_mutex_lock(&list->lock);
    if (result)
        return NOAQUIRE; /* Cannot aquire lock */
    return 0;
}

int list_unlock(List* list)
{
    int result;
    result = pthread_mutex_unlock(&list->lock);
    if (result)
        return NORELEASE; /* Cannot release lock */
    return 0;
}

int list_append(List* list, void* item)
{
    int result;

    result = list_lock(list);
    if (result)
        return NOAQUIRE; /* Cannot aquire lock */

    /* Realloc */
    if (list->length == list->size) {
        list->data = (void**)realloc(list->data, (list->size * 2) * sizeof(void*));
        if (list->data == NULL) {
            perror("list_append");
            result = list_unlock(list);
            if (result)
                return NORELEASE; /* Cannot release lock */
            return NOALLOCATE; /* Memory allocation failed */
        }
        list->size = list->size * 2;
    } else if (list->length > list->size) {
        list_unlock(list);
        if (result)
            return NORELEASE; /* Cannot release lock */
        return UNKOWN; /* Sanity check */
    }

    /* Append item */
    list->data[list->length] = item;
    list->length++;

    list_unlock(list);
    if (result)
        return NORELEASE; /* Cannot release lock */
    return 0;
}

void* list_pop(List* list)
{
    int result;

    result = list_lock(list);
    if (result)
        return NULL; /* Cannot aquire lock */

    /* Pop item */
    if (!list->length) {
        list_unlock(list);
        if (result)
            return NULL; /* Cannot release lock */
        return NULL;
    }
    void* item=list->data[list->length - 1];
    list->data[list->length - 1] = NULL; /* This is not needed but useful */
    list->length--;

    /* Realloc */
    if (list->size / 2 >= MIN_SIZE && list->length < list->size / 2) {
        list->data = (void**)realloc(list->data, (list->size / 2) * sizeof(void*));
        if (list->data == NULL) {
            perror("list_pop");
            list_unlock(list);
            if (result)
                return NULL; /* Cannot release lock */
            return NULL; /* Memory allocation failed */
        }
        list->size = list->size / 2;
    }

    list_unlock(list);
    if (result)
        return NULL; /* Cannot release lock */
    return item;
}

int list_remove(List* list, size_t index)
{
    int result;

    result = list_lock(list);
    if (result)
        return NOAQUIRE; /* Cannot aquire lock */

    /* Remove and shift */
    for(int i=index; i < list->length - 1; i++)
        list->data[i] = list->data[i + 1];
    list->length--;

    /* Realloc */
    if (list->size / 2 >= MIN_SIZE && list->length < list->size / 2) {
        list->data = (void**)realloc(list->data, (list->size / 2) * sizeof(void*));
        if (list->data == NULL) {
            perror("list_pop");
            list_unlock(list);
            if (result)
                return NORELEASE; /* Cannot release lock */
            return NOALLOCATE; /* Memory allocation failed */
        }
        list->size = list->size / 2;
    }

    list_unlock(list);
    if (result)
        return NORELEASE; /* Cannot release lock */
    return 0;
}

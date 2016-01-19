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

list_result init_list_size(List* list, size_t size)
{
    int result;

    result = pthread_mutex_init(&list->lock, NULL);
    if (result)
        return NOINIT;
    list->data = (void**)malloc(size * sizeof(void*));
    if (list->data == NULL) {
        perror("init_list_size");
        return NOALLOCATE;
    }
    list->size = size;
    list->length = 0;
    return SUCCESS;
}

list_result init_list(List* list)
{
    return init_list_size(list, MIN_SIZE);
}

list_result destroy_list(List* list)
{
    int result;

    /* Free data if we can aquire the lock */
    result = pthread_mutex_trylock(&list->lock);
    if (result == EBUSY) {
        return HELD;
    } else if (result) {
        return NOAQUIRE;
    }
    free(list->data);

    /* Destroy the lock */
    result = pthread_mutex_unlock(&list->lock);
    if (result)
        return NORELEASE;
    result = pthread_mutex_destroy(&list->lock);
    if (result)
        return NODESTROY;

    return SUCCESS;
}

list_result list_lock(List* list)
{
    int result;
    result = pthread_mutex_lock(&list->lock);
    if (result)
        return NOAQUIRE;
    return SUCCESS;
}

list_result list_unlock(List* list)
{
    int result;
    result = pthread_mutex_unlock(&list->lock);
    if (result)
        return NORELEASE;
    return SUCCESS;
}

list_result list_append(List* list, void* item)
{
    int result;

    result = list_lock(list);
    if (result)
        return NOAQUIRE;

    /* Realloc */
    if (list->length == list->size) {
        list->data = (void**)realloc(list->data, (list->size * 2) * sizeof(void*));
        if (list->data == NULL) {
            perror("list_append");
            return NOALLOCATE;
        }
        list->size = list->size * 2;
    }

    /* Append item */
    list->data[list->length] = item;
    list->length++;

    result = list_unlock(list);
    if (result)
        return NORELEASE;
    return SUCCESS;
}

void* list_pop(List* list, list_result* error)
{
    int result;

    result = list_lock(list);
    if (result) {
        *error = NOAQUIRE;
        return NULL;
    }

    /* Pop item */
    if (!list->length) {
        *error = LISTEMPTY;
        result = list_unlock(list);
        if (result)
            *error = NORELEASE;
        return NULL;
    }
    list->length--;
    void* item=list->data[list->length];

    /* Realloc */
    if (list->size / 2 >= MIN_SIZE && list->length < list->size / 2) {
        list->data = (void**)realloc(list->data, (list->size / 2) * sizeof(void*));
        if (list->data == NULL) {
            perror("list_pop");
            *error = NOALLOCATE;
            return NULL;
        }
        list->size = list->size / 2;
    }

    result = list_unlock(list);
    if (result) {
        *error = NORELEASE;
        return NULL;
    }
    return item;
}

list_result list_remove(List* list, size_t index)
{
    int result;

    result = list_lock(list);
    if (result)
        return NOAQUIRE;

    /* Remove and shift */
    for(int i=index; i < list->length - 1; i++)
        list->data[i] = list->data[i + 1];
    list->length--;

    /* Realloc */
    if (list->size / 2 >= MIN_SIZE && list->length < list->size / 2) {
        list->data = (void**)realloc(list->data, (list->size / 2) * sizeof(void*));
        if (list->data == NULL) {
            perror("list_remove");
            return NOALLOCATE;
        }
        list->size = list->size / 2;
    }

    result = list_unlock(list);
    if (result)
        return NORELEASE;
    return SUCCESS;
}

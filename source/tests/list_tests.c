/*
 * Thread-safe list/vector implementation unit-tests
 *
 * By: Khile Dupuis
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"


void print_list(List* list)
{
    printf("List:\n");
    for(long int i=0; i < list->length; i++) {
        printf("%li:", i);
        printf("%s\n", (char*)list->data[i]);
    }
}

void print_error(int error)
{
    switch(error)
    {
        case NORELEASE:
            fprintf(stderr, "NORELEASE\n");
        case NOAQUIRE:
            fprintf(stderr, "NORELEASE\n");
        case HELD:
            fprintf(stderr, "NORELEASE\n");
        case NODESTROY:
            fprintf(stderr, "NORELEASE\n");
        case NOINIT:
            fprintf(stderr, "NORELEASE\n");
        case NOALLOCATE:
            fprintf(stderr, "NORELEASE\n");
        case LISTEMPTY:
            fprintf(stderr, "NORELEASE\n");
        default:
            assert(1);
    }
}


int main(int argc, char** argv)
{
    int result;
    list_result error;

    int size;
    List list;
    void* item;

    int string_count=10;
    char* strings[10]={
        "",
        "1",
        "22",
        "333",
        "4444",
        "55555",
        "666666",
        "7777777",
        "88888888",
        "999999999"};

    /* init_list test */
    printf("init_list tests...\n");
    result = init_list(&list);
    if (result)
        return -1;
    assert(list.size == LIST_MIN_SIZE);
    assert(list.length == 0);

    /* list_append test */
    printf("list_append tests...\n");
    for (int i=0; i < string_count; i++) {
        result = list_append(&list, strings[i]);
        assert(!result);
    }
    size = LIST_MIN_SIZE;
    while (size < string_count)
        size = size * LIST_GROWTH;
    assert(list.size == size);
    assert(list.length == string_count);

    /* list_pop test */
    printf("list_pop tests...\n");
    for (int i=0; i < string_count; i++) {
        item = list_pop(&list, &error);
        assert(item != NULL);
    }
    assert(list.size == LIST_MIN_SIZE);
    assert(list.length == 0);
    item = list_pop(&list, &error);
    assert(item == NULL);

    /* list_append test */
    printf("list_append tests...\n");
    for (int i=0; i < string_count; i++) {
        result = list_append(&list, strings[i]);
        assert(!result);
    }
    size = LIST_MIN_SIZE;
    while (size < string_count) {
        size = size * LIST_GROWTH;

    }
    assert(list.size == size);
    assert(list.length == string_count);

    /* list_remove test */
    printf("list_remove tests...\n");
    result = list_remove(&list, 1);
    assert(!result);
    assert(list.length == string_count - 1);
    assert(list.data[1] == strings[2]);
    while (list.length != 0) {
        result = list_remove(&list, list.length - 1);
        assert(!result);
    }
    assert(list.length == 0);
    assert(list.size == LIST_MIN_SIZE);

    destroy_list(&list); /* Cleaning the kitchen before tearing down the house */
    return 0;
}

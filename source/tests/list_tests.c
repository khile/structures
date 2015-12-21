/*
 * Thread-safe list/vector implementation unit-tests
 *
 * By: Khile Dupuis
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "list.h"


void print_list(List* list)
{
    printf("List:\n");
    for(long int i=0; i < list->length; i++) {
        printf("%li:", i);
        printf("%s\n", (char*)list->data[i]);
    }
}


int main(int argc, char** argv)
{
    int result;
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
    List list;
    void* item;

    /* init_list test */
    printf("init_list tests...\n");
    result = init_list(&list);
    if (result)
        return -1;
    print_list(&list);
    assert(list.size == MIN_SIZE);
    assert(list.length == 0);

    /* list_append test */
    printf("list_append tests...\n");
    for (int i=0; i < string_count; i++) {
        result = list_append(&list, strings[i]);
        if (result)
            return -1;
    }
    int size=MIN_SIZE;
    while (size < string_count)
        size = size * 2;
    print_list(&list);
    assert(list.size == size);
    assert(list.length == string_count);

    /* list_pop test */
    printf("list_pop tests...\n");
    for (int i=0; i < string_count; i++) {
        item = list_pop(&list);
        if (item == NULL)
            fprintf(stderr, "Error popping\n");
        print_list(&list);
    }
    printf("%li\n", list.size);
    assert(list.size == MIN_SIZE);
    assert(list.length == 0);
    item = list_pop(&list);
    assert(item == NULL);

    destroy_list(&list); /* Cleaning the kitchen before tearing down the house */
    return 0;
}

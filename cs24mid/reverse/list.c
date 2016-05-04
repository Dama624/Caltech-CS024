#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Appends a value onto the end of the linked list. */
void append_value(LinkedList *list, int value) {
    ListNode *node;

    assert(list != NULL);

    node = malloc(sizeof(ListNode));
    node->value = value;
    node->next = NULL;

    if (list->tail != NULL)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
}


/* Prints a linked list out to standard output. */
void print_list(const LinkedList *list) {
    int first;
    const ListNode *node;

    assert(list != NULL);

    printf("[");
    node = list->head;
    first = 1;
    
    while (node != NULL) {
        if (first)
            first = 0;
        else
            printf(", ");

        printf("%d", node->value);

        node = node->next;
    }

    printf("]\n");
}


/* Frees the contents of a linked list.  Also overwrites the nodes with 0xcc
 * to identify access-after-free bugs.
 */
void free_list(LinkedList *list) {
    ListNode *node;

    node = list->head;
    while (node != NULL) {
        ListNode *next = node->next;
        free(node);
        memset(node, 0xcc, sizeof(ListNode));
        node = next;
    }

    list->head = NULL;
    list->tail = NULL;
}


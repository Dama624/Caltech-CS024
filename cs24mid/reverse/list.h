#ifndef LIST_H
#define LIST_H


/* An individual node in a singly linked list. */
typedef struct ListNode {
    int value;
    struct ListNode *next;
} ListNode;


/* The structure that keeps track of the start and end of the linked list. */
typedef struct LinkedList {
    ListNode *head;
    ListNode *tail;
} LinkedList;


void append_value(LinkedList *list, int value);
void print_list(const LinkedList *list);
void free_list(LinkedList *list);

void reverse_list(LinkedList *list);

#endif /* LIST_H */


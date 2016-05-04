#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"


/* The total number of values to generate for testing. */
#define NUM_VALUES 13


/* A simple test program that performs a few tests on reverse_list() to see
 * if it does what it's supposed to do.
 */
int main() {
    int i, fail;
    int values[NUM_VALUES];
    LinkedList list = {NULL, NULL};
    ListNode *node;

    fail = 0;

    /* Try to reverse an empty linked list - see what happens with that! */
    reverse_list(&list);
    if (list.head != NULL || list.tail != NULL) {
        printf("ERROR:  Reversing an empty list mangles it!\n");
        fail = 1;
        list.head = NULL;
        list.tail = NULL;
    }

    /* Seed the random number generator so that things change each run. */
    srand(time(NULL));

    /* Generate an array of random values.  Also, store these values into
     * the linked list.
     */
    for (i = 0; i < NUM_VALUES; i++) {
        values[i] = rand() % 100;
        append_value(&list, values[i]);
    }
    printf("Initial list:  ");
    print_list(&list);

    /* Try to reverse the contents of the linked list. */
    reverse_list(&list);

    printf("Reversed list:  ");
    print_list(&list);

    /* Iterate over the list and see if the contents are actually reversed. */
    for (i = 0, node = list.head; i < NUM_VALUES; i++, node = node->next) {
        if (node == NULL) {
            printf("ERROR:  Linked list doesn't have all expected values!\n");
            fail = 1;
            break;
        }

        int expected = values[NUM_VALUES - i - 1];
        if (node->value != expected) {
            printf("ERROR:  Value at node %d should be %d; saw %d instead.\n",
                   i, expected, node->value);
            fail = 1;
        }
    }

    if (i != NUM_VALUES) {
        printf("ERROR:  List terminated early.\n");
        fail = 1;
    }

    /* All done testing, print out the verdict. */
    if (fail) {
        printf("\nFailures detected.\n");
    }
    else {
        printf("No failures detected.  Good job.\n");
    }

    free_list(&list);

    return 0;
}


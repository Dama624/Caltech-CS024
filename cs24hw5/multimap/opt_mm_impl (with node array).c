#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multimap.h"

#define MAX_LEN 14


/*============================================================================
 * TYPES
 *
 *   These types are defined in the implementation file so that they can
 *   be kept hidden to code outside this source file.  This is not for any
 *   security reason, but rather just so we can enforce that our testing
 *   programs are generic and don't have any access to implementation details.
 *============================================================================*/

 /* Represents an array of values that is associated with a given key in the
  * multimap. */
typedef struct multimap_value {
    unsigned int count;
    int value[MAX_LEN];
    struct multimap_value *next;
} multimap_value;


/* Represents a key and its associated values in the multimap, as well as
 * pointers to the left and right child nodes in the multimap. */
typedef struct multimap_node {
    /* The key-value that this multimap node represents. */
    int key;

    /* A linked list of the values associated with this key in the multimap. */
    multimap_value *values;

    /* The last of the linked list of values, so that we can retain the order
     * they were added to the list.
     */
    multimap_value *values_tail;

    /* The left child of the multimap node.  This will reference nodes that
     * hold keys that are strictly less than this node's key.
     */
    struct multimap_node *left_child;

    /* The right child of the multimap node.  This will reference nodes that
     * hold keys that are strictly greater than this node's key.
     */
    struct multimap_node *right_child;
} multimap_node;


/* The entry-point of the multimap data structure. */
struct multimap {
    unsigned int len; // length of multimap_node array
    multimap_node *root;
};


/*============================================================================
 * HELPER FUNCTION DECLARATIONS
 *
 *   Declarations of helper functions that are local to this module.  Again,
 *   these are not visible outside of this module.
 *============================================================================*/

void realloc_mm_array(multimap * mm);

multimap_node * find_mm_node(multimap *mm, int key,
                             int create_if_not_found);

void free_multimap_values(multimap_value *values);


/*============================================================================
 * FUNCTION IMPLEMENTATIONS
 *============================================================================*/

/* Allocates a multimap node, and zeros out its contents so that we know what
 * the initial value of everything will be.
 */
void realloc_mm_array(multimap * mm) {
    unsigned newlen;
    int i;

    newlen = (2 * mm->len) + 1;
    mm->root = (multimap_node *)realloc(mm->root, newlen
                                                  * sizeof(multimap_node));
    for (i = mm->len; i < newlen; i++)
    {
        mm->root[i].key = -1;
    }
    mm->len = newlen;
}


/* This helper function searches for the multimap node that contains the
 * specified key.  If such a node doesn't exist, the function can initialize
 * a new node and add this into the structure, or it will simply return NULL.
 * The one exception is the root - if the root is NULL then the function will
 * return a new root node.
 */
multimap_node * find_mm_node(multimap *mm, int key,
                             int create_if_not_found) {
    multimap_node *result;
    unsigned int curr_offset;
    unsigned int new_offset;

    /* If the entire multimap is empty, the root will have value -1. */
    if (mm->root[0].key == -1) {
        if (create_if_not_found) {
            (mm->root)->key = key;
        }
        return mm->root;
    }

    /* Now we know the multimap has at least a root node, so start there. */
    curr_offset = 0;
    while (1) {
        if (((mm->root)[curr_offset]).key == key)
            break;

        if ((mm->root)[curr_offset].key > key) {   /* Follow left child */
            new_offset = (2 * (curr_offset + 1)) - 1;
            // Check if going left goes out of array bounds
            if (new_offset + 1 > mm->len && create_if_not_found) {
                // goes out of bounds, we need to realloc
                realloc_mm_array(mm);
            }
            if ((mm->root)[new_offset].key == -1 && create_if_not_found) {
                // within bounds; no need to realloc
                (mm->root)[curr_offset].left_child = (mm->root) + new_offset;
                (mm->root)[curr_offset].left_child->key = key;
            }
            curr_offset = new_offset;
        }
        else {                   /* Follow right child */
            new_offset = 2 * (curr_offset + 1);
            // Check if going right goes out of bounds
            if (new_offset + 1 > mm->len && create_if_not_found) {
                // goes out of bounds, realloc
                realloc_mm_array(mm);
            }
            if ((mm->root)[new_offset].key == -1 && create_if_not_found) {
                // within bounds; no need to realloc
                (mm->root)[curr_offset].right_child = (mm->root) + new_offset;
                (mm->root)[curr_offset].right_child->key = key;
            }
            curr_offset = new_offset;
        }

        if (curr_offset + 1 > mm->len)
        {
            return NULL;
        }
    }
    result = (mm->root) + curr_offset;
    return result;
}


/* This helper function frees all values in a multimap node's value-list. */
void free_multimap_values(multimap_value *values) {
    while (values != NULL) {
        multimap_value *next = values->next;
#ifdef DEBUG_ZERO
        /* Clear out what we are about to free, to expose issues quickly. */
        bzero(values, sizeof(multimap_value));
#endif
        free(values);
        values = next;
    }
}

/* Initialize a multimap data structure. */
multimap * init_multimap() {
    int i;

    multimap *mm = malloc(sizeof(multimap));
    mm->len = 10; // Initialize with space for 10 nodes
    mm->root = (multimap_node *)malloc(sizeof(multimap_node) * 10);
    bzero(mm->root, sizeof(multimap_node) * 10);
    for (i = 0; i < mm->len; i++)
    {
        mm->root[i].key = -1;
    }
    return mm;
}


/* Release all dynamically allocated memory associated with the multimap
 * data structure.
 */
void clear_multimap(multimap *mm) {
    int i;

    assert(mm != NULL);
    // first free every node's value list
    for (i = 0; i < mm->len; i++)
    {
        free_multimap_values(mm->root[i].values);
    }
    // then free the array of nodes
    free(mm->root);
}


/* Adds the specified (key, value) pair to the multimap. */
void mm_add_value(multimap *mm, int key, int value) {
    multimap_node *node;
    multimap_value *new_value;
    unsigned int count;

    assert(mm != NULL);

    /* Look up the node with the specified key.  Create if not found. */
    node = find_mm_node(mm, key, /* create */ 1);

    assert(node != NULL);
    assert(node->key == key);

    /* Add the new value to the multimap node. */
    // Check if value array exists & can hold another value
    if (node->values_tail != NULL &&
        (node->values_tail)->count < MAX_LEN)
    {
        count = (node->values_tail)->count;
        // Add value to this array
        (node->values_tail)->value[count] = value;
        (node->values_tail)->count += 1;
    }
    // does not exist or can't hold another value
    else
    {
        // Create a new array
        new_value = malloc(sizeof(multimap_value));
        new_value->count = 1;
        new_value->value[0] = value;
        new_value->next = NULL;
        if (node->values_tail != NULL &&
            (node->values_tail)->count >= MAX_LEN)
        {
            // array exists but can't hold another value
            // make current array's next point to new array
            (node->values_tail)->next = new_value;
        }
        if (node->values == NULL)
        {
            // first time initializing an array
            node->values = new_value;
        }
        node->values_tail = new_value;
    }
}


/* Returns nonzero if the multimap contains the specified key-value, zero
 * otherwise.
 */
int mm_contains_key(multimap *mm, int key) {
    return find_mm_node(mm, key, /* create */ 0) != NULL;
}


/* Returns nonzero if the multimap contains the specified (key, value) pair,
 * zero otherwise.
 */
int mm_contains_pair(multimap *mm, int key, int value) {
    multimap_node *node;
    multimap_value *curr;
    int i;

    node = find_mm_node(mm, key, /* create */ 0);
    if (node == NULL)
        return 0;

    curr = node->values;
    while (curr != NULL) {
        for (i = 0; i < curr->count; i++)
        {
            if (curr->value[i] == value)
            {
                return 1;
            }
        }
        curr = curr->next;
    }

    return 0;
}


/* This helper function is used by mm_traverse() to traverse every pair within
 * the multimap.
 */
void mm_traverse_helper(multimap_node *node, void (*f)(int key, int value)) {
    multimap_value *curr;
    int i;

    if (node == NULL)
        return;

    mm_traverse_helper(node->left_child, f);

    curr = node->values;
    while (curr != NULL) {
        for (i = 0; i < curr->count; i++)
        {
            f(node->key, curr->value[i]);
        }
        curr = curr->next;
    }

    mm_traverse_helper(node->right_child, f);
}


/* Performs an in-order traversal of the multimap, passing each (key, value)
 * pair to the specified function.
 */
void mm_traverse(multimap *mm, void (*f)(int key, int value)) {
    mm_traverse_helper(mm->root, f);
}


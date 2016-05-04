.globl reverse_list
#============================================================================
# reverse_list:  Reverses the order of an input linked list. Done in-place.
#
# Author:  Hong Joon Park
#
# The argument to reverse_list is at this stack location:
#
#      8(%ebp) = contains address of an input LinkedList struct
#
# %eax will be used to store the address of the head of a linked list.
# %ebx will be used to store the address of a temporary linked list node.
#
# This function has no return value. It directly modifies the input list.
#
reverse_list:
        # Set up stack frame.
        pushl   %ebp
        movl    %esp, %ebp
        # Initialize Linkedlist node *new->head
        xor     %eax, %eax
        # Initialize LinkedNode *element
        xor     %ebx, %ebx
        # Extract the address of the head of the input linked list
        movl    8(%ebp), %ecx # ecx now contains address of linked list struct
        movl    (%ecx), %edx  # edx contains address of linked list head
        # Set linked list head to tail
        movl    %edx, 4(%ecx)
        # Initiate the while loop... check condition first
        cmpl    $0, %edx      # Check if linked list head is NULL
        je      END

WHILE_LOOP:
        movl    %edx, %ebx    # "Remove" the original list's current head node
        movl    4(%edx), %ecx # ecx now contains list->head->next
        movl    %ecx, %edx    # Set list->head node to its next node
        movl    %eax, 4(%ebx) # Make the current new list's head the "next"
        movl    %ebx, %eax    # Set new list head to removed node
        cmpl    $0, %edx
        jne     WHILE_LOOP

END:
        movl    8(%ebp), %ecx # ecx now contains address of linked list struct
        movl    (%ecx), %edx  # edx contains address of linked list head
        # Set new linked list head
        movl    %eax, (%ecx)
        # Clean up stack frame & return
        movl     %ebp, %esp
        popl     %ebp
        ret

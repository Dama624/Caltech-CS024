#
# compare_and_swap(uint32_t *target, uint32_t old, uint32_t new)
#
# compare_and_swap compares the value of *target to old, and if they are the
# same, it stores new into *target and returns 1.  Otherwise, the function 
# does not modify *target, and returns 0.
#
# The arguments are located in the following:
#    *target : 8(%ebp)
#    old     : 12(%ebp)
#    new     : 16(%ebp)
#
.globl compare_and_swap
compare_and_swap:
    # Set up stack frame
    pushl   %ebp
    movl    %esp, %ebp

    # Store $1 into eax
    xor     %eax, %eax
    addl    $1, %eax
    pushl   %eax
    # Store 0 into edx
    xor     %edx, %edx
    # Store values old and new into eax and ebx respectively
    movl    12(%ebp), %eax
    movl    16(%ebp), %ebx
    # Store address of target into ecx
    movl    8(%ebp), %ecx
    # lock and perform the compare and exchange
    lock
    # compare value in address in ecx to eax (currently old)
    cmpxchg %ebx, (%ecx)
    # if equal, new stored into target & ZF = 1; otherwise ZF = 0
    # thus if ZF = 1, return 1; otherwise return 0
    popl    %eax
    cmovnz  %edx, %eax

    # Clean up stack frame & return
    movl    %ebp, %esp
    popl    %ebp
    ret

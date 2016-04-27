.globl my_setjmp
#============================================================================
# my_setjmp:  Emulates the standard C setjmp() function. Stores the current
#             execution state into my_jmp_buf, then returns 0.
#
# Author:  Hong Joon Park
#
# The argument to my_setjmp is at this stack location:
#
#      8(%ebp) = address of the start of an empty array my_jmp_buf
#
# my_jmp_buf stores values in this order:
#
#      Offset: 0   4   8   12  16  20
#	   Value : ebx esi edi esp ebp RetAddr
#
# Return-value in %eax is 0, indicating that the execution state has been
# saved into my_jmp_buf.
#
my_setjmp:
        # Set up stack frame.
        pushl   %ebp
        movl    %esp, %ebp
        # Begin saving execution state
        movl    8(%ebp), %ecx       # Address of my_jmp_buf in ecx
        movl    %ebx, (%ecx)        # Move ebx to my_jmp_buf
        movl    %esi, 4(%ecx)       # Move esi
        movl    %edi, 8(%ecx)       # And so on...
        movl    %esp, 12(%ecx)
        movl    (%esp), %edx        
        movl    %edx, 16(%ecx)      # ebp currently stored where esp is at
        movl    4(%ebp), %edx   
        movl    %edx, 20(%ecx)      # RetAddr stored in 4(%ebp)
        # Set return value to 0
        movl    $0, %eax
        # Clean up stack frame & return
        movl     %ebp, %esp
        popl     %ebp
        ret

.globl my_longjmp
#============================================================================
# my_longjmp:  Emulates the standard C longjmp() function. Restores the
#              execution state stored within my_jmp_buf. 
#
# Author:  Hong Joon Park
#
# The arguments to my_longjmp are at these stack locations:
#
#      8(%ebp) = address of the start of a filled array my_jmp_buf
#     12(%ebp) = int to return. Represents an error throw.
#
# my_jmp_buf stores values as described in my_setjmp.
#
# Return value stored in %eax. The value should equal 12(%ebp). 
#
my_longjmp:
        # Set up stack frame.
        pushl   %ebp
        movl    %esp, %ebp
        # Move register values back
        movl    8(%ebp), %ecx       # Address of my_jmp_buf in ecx
        movl    (%ecx), %ebx        # Restore ebx
        movl    4(%ecx), %esi       # Restore esi
        movl    8(%ecx), %edi       # Restore edi
        # Set return value (doing so now before we lose our current frame)
        # Check if 12(%ebp) = 0
        cmpl    $0, 12(%ebp)
        je     is_zero
        movl    12(%ebp), %eax
        jne     finish
is_zero:
        movl    $1, %eax            # Return 1 if 12(%ebp) = 0
finish:
        # Restore the stack [frame]
        movl    12(%ecx), %esp      # Restore stack pointer esp
        movl    %esp, %ebp          # Move ebp to restored esp
        movl    16(%ecx), %edx      # Restore ebp
        movl    %edx, (%esp)        # Restore ebp
        movl    20(%ecx), %edx      # Restore RetAddr
        movl    %edx, 4(%ebp)       # Restore RetAddr
        # Clean up stack frame & return
        movl     %ebp, %esp
        popl     %ebp
        ret

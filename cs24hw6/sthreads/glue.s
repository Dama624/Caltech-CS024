#
# Keep a pointer to the main scheduler context.
# This variable should be initialized to %esp;
# which is done in the __sthread_start routine.
#
        .data
        .align 4
scheduler_context:      .long   0

#
# __sthread_schedule is the main entry point for the thread
# scheduler.  It has three parts:
#
#    1. Save the context of the current thread on the stack.
#       The context includes only the integer registers
#       and EFLAGS.
#    2. Call __sthread_scheduler (the C scheduler function),
#       passing the context as an argument.  The scheduler
#       stack *must* be restored by setting %esp to the
#       scheduler_context before __sthread_scheduler is
#       called.
#    3. __sthread_scheduler will return the context of
#       a new thread.  Restore the context, and return
#       to the thread.
#
        .text
        .align 4
        .globl __sthread_schedule
__sthread_schedule:

        # Save the process state onto its stack
        pushl   %eax
        pushl   %ebx
        pushl   %ecx
        pushl   %edx
        pushl   %esp
        pushl   %ebp
        pushl   %esi
        pushl   %edi
        pushfl

        # Call the high-level scheduler with the current context as an argument
        movl    %esp, %eax
        movl    scheduler_context, %esp
        pushl   %eax
        call    __sthread_scheduler

        # The scheduler will return a context to start.
        # Restore the context to resume the thread.
__sthread_restore:
        movl    %eax, %esp
        popfl
        popl    %edi
        popl    %esi
        popl    %ebp
        popl    %esp
        popl    %edx
        popl    %ecx
        popl    %ebx
        popl    %eax
        ret

#
# Initialize a process context, given:
#    1. the stack for the process (8(%ebp))
#    2. the function to start (12(%ebp))
#    3. its argument (16(%ebp))
# The context should be consistent with the context
# saved in the __sthread_schedule routine.
#
# A pointer to the newly initialized context is returned to the caller.
# (This is the stack-pointer after the context has been set up.)
#
# This function is described in more detail in sthread.c.
#
#
        .globl __sthread_initialize_context
__sthread_initialize_context:

        # Set our stack pointer as our current base (to retrieve arguments)
        # movl    %esp, %ebp
        pushl %ebp
        movl  %esp, %ebp
        # Now relocate stack pointer to allocated region
        movl    8(%ebp), %esp
        # Move function argument
        movl    16(%ebp), %eax
        # Move function
        movl    12(%ebp), %ebx
        # First push the function argument
        pushl   %eax
        # Push __sthread_finish()
        pushl   $__sthread_finish
        # Push the function itself
        pushl   %ebx


        # Now we start pushing dummy registers in proper order (except esp)
        pushl   %esp            # eax
        pushl   $0              # ebx
        pushl   $0              # ecx
        pushl   $0              # edx
        pushl   %esp            # esp
        pushl   $0              # ebp
        pushl   $0              # esi
        pushl   $0              # edi
        pushfl                  # EFLAGS
        
        # Move current stack pointer to eax to return it
        movl    %esp, %eax

        # Clean stack
        movl %ebp, %esp
        popl %ebp
        ret

#
# The start routine initializes the scheduler_context variable,
# and calls the __sthread_scheduler with a NULL context.
# The scheduler will return a context to resume.
#
        .globl __sthread_start
__sthread_start:
        # Remember the context
        movl    %esp, scheduler_context

        # Call the scheduler with no context
        pushl   $0
        call    __sthread_scheduler

        # Restore the context returned by the scheduler
        jmp     __sthread_restore

################################################
# Data section.
#

#
# Keep a pointer to the main scheduler context.
# This variable should be initialized to %esp;
# which is done in the __sthread_start routine.
#
        .data
        .align 4
scheduler_context:      .long   0

#
# Integer variable for locking the scheduler
#
scheduler_lock:         .long   0

################################################
# Code section.
#
        .text
        .align 4

#
# This function can be called from C to obtain the scheduler lock.
# It returns 1 if the lock is granted, 0 otherwise.
#
        .globl __sthread_lock
__sthread_lock:
        # Atomically store scheduler_lock into CF then set scheduler_lock = 1
        lock
        bts     $0, scheduler_lock
        # First zero out return register eax
        xor     %eax, %eax
        # If CF = 1, scheduler_lock was 1, so return 0 (already done)
        # If CF = 0, return 1
        setnc   %al
        # setnc sets the lower byte of eax, mask with 1 for 1 bit
        and     $1, %eax
        # will return 1 if scheduler_lock = 0, and 0 if scheduler_lock = 1
        ret

        .globl __sthread_unlock
__sthread_unlock:
        movl    $0, scheduler_lock
        ret

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
        .globl __sthread_schedule
__sthread_schedule:
        # Save the process state onto its stack
        pushfl
        pusha

        # Call the high-level scheduler with the current context as an argument
        movl    %esp, %eax
        movl    scheduler_context, %esp
        pushl   %eax
        call    __sthread_scheduler

        # The scheduler will return a context to start.
        # Restore the context to resume the thread.
__sthread_restore:
        # Unlock thread after context switch
        call    __sthread_unlock
        movl    %eax, %esp
        popa
        popfl        
        ret

#
# Initialize a process context, given:
#    1. the stack for the process
#    2. the function to start
#    3. its argument
# The context should be consistent with the context
# saved in the __sthread_schedule routine.
#
        .globl __sthread_initialize_context
__sthread_initialize_context:
        # Get the arguments
        movl    4(%esp), %eax   # Stack-pointer for the process
        movl    8(%esp), %ecx
        movl    12(%esp), %edx

        # The final context pointer after the context has been
        # initialized will be in %eax.  (This is also why we don't
        # really care about initializing %esp on the stack properly;
        # it will be overwritten when we resume the thread.)
        subl    $48, %eax

        # Initialize the context itself.
        movl    $0, 0(%eax)     # General-purpose registers.
        movl    $0, 4(%eax)
        movl    $0, 8(%eax)
        movl    $0, 12(%eax)
        movl    $0, 16(%eax)
        movl    $0, 20(%eax)
        movl    $0, 24(%eax)
        movl    $0, 28(%eax)
        movl    $0, 32(%eax)    # EFLAGS
        movl    %ecx, 36(%eax)  # return address
        movl    $__sthread_finish, 40(%eax)  # return addr from called thread
        movl    %edx, 44(%eax)  # argument

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
        # Lock the thread before scheduling
        call    __sthread_lock
        call    __sthread_scheduler

        # Restore the context returned by the scheduler
        jmp     __sthread_restore


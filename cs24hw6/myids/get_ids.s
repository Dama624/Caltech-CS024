#=============================================================================
# int getuid()
#
#     Retrieves the user ID from the kernel.
#
# Arguments:
#     None
#
# Returns:
#     The current user ID as an int.
#
.globl getuid
getuid:
    movl  $199, %eax         # __NR_getuid32 199
    int   $0x80             # Invoke system call
    ret                     # ID stored in %eax

#=============================================================================
# int getgid()
#
#     Retrieves the group ID from the kernel.
#
# Arguments:
#     None
#
# Returns:
#     The current group ID as an int.
#
.globl getgid
getgid:
    movl  $200, %eax        # __NR_getgid32 200
    int   $0x80             # Invoke system call
    ret                     # ID stored in %eax

#=============================================================================
# void get_ids(int *uid, int *gid)
#
#     Takes in two addresses to store user and group ID, then stores the
#     respective values in the addresses.
#
# Arguments:
#     int *uid: The address into which the user ID will be stored
#     int *gid: The address into which the group ID will be stored
#
# Returns:
#     Nothing; the IDs will be stored into their respective input addresses
#
.globl get_ids
get_ids:
    # Set up stack frame
    pushl %ebp
    movl  %esp, %ebp
    # Store the address of uid
    movl 8(%ebp), %ebx
    call  getuid
    # user ID now stored in eax
    movl  %eax, (%ebx)
    # Store the address of gid
    movl 12(%ebp), %ebx
    call  getgid
    # group ID now stored in eax
    movl  %eax, (%ebx)
    # Clear stack
    movl %ebp, %esp
    popl %ebp
    ret

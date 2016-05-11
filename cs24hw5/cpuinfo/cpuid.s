.text


#=============================================================================
# unsigned int cpuid_0(char *vendor_string)
#
#     Invokes the CPUID instruction with %eax set to 0, to retrieve the CPU's
#     vendor-string and the maximum value that can be used with CPUID.
#
# Arguments:
#     vendor_string A pointer to where the vendor-string should be stored.
#         This must point to a region of memory of 13 bytes in size, or more.
#
# Returns:
#     The maximum value that can be used with the CPUID instruction.
#
.globl _cpuid_0
.globl cpuid_0
_cpuid_0:
cpuid_0:
    pushl %ebp
    movl  %esp, %ebp

    pushl %ebx
    pushl %edi

    # Get the maximum input value for basic CPUID information

    xorl %eax, %eax
    cpuid

    # eax will contain the max value that can be passed to cpuid,
    # so we'll return that value.  The other registers contain the
    # vendor string; store that into the location pointed to by
    # the argument.

    pushl %eax               # Save this value for later

    movl 8(%ebp), %edi
    movl %ebx, (%edi)
    movl %edx, 4(%edi)
    movl %ecx, 8(%edi)
    movb $0, 12(%edi)

    # Get the maximum input value for extended CPUID information

    movl $0x80000000, %eax
    cpuid

    # eax will contain the max value for getting extended function information

    movl 12(%ebp), %edi
    movl %eax, (%edi)

    # Restore the max CPUID value for basic information, to return it
    popl %eax

    popl %edi
    popl %ebx

    movl %ebp, %esp
    popl %ebp
    ret


#=============================================================================
# void cpuid_4(unsigned int ecx, regs_t *regs)
#
#     Invokes the CPUID instruction with %eax = 4, and %ecx set to the
#     specified value.  The results are stored into regs.
#
.globl _cpuid_4
.globl cpuid_4
_cpuid_4:
cpuid_4:
    pushl %ebp
    movl  %esp, %ebp

    pushl %ebx
    pushl %edi

    # Invoke CPUID with eax = 4 and ecx set to the specified value.
    movl $4, %eax
    movl 8(%ebp), %ecx
    cpuid

    # Store the register contents for the caller to use.
    movl 12(%ebp), %edi
    movl %eax,   (%edi)
    movl %ebx,  4(%edi)
    movl %ecx,  8(%edi)
    movl %edx, 12(%edi)

    popl %edi
    popl %ebx

    movl %ebp, %esp
    popl %ebp
    ret


#=============================================================================
# void cpuid_n(unsigned int eax, regs_t *regs)
#
#     Invokes the CPUID instruction with the specified value of %eax, and
#     stores the results into regs.
#
.globl _cpuid_n
.globl cpuid_n
_cpuid_n:
cpuid_n:
    pushl %ebp
    movl  %esp, %ebp

    pushl %ebx
    pushl %edi

    # Invoke CPUID with the specified value for %eax
    movl  8(%ebp), %eax
    cpuid

    # Store the results into the target location
    movl  12(%ebp), %edi
    movl %eax,   (%edi)
    movl %ebx,  4(%edi)
    movl %ecx,  8(%edi)
    movl %edx, 12(%edi)

    popl %edi
    popl %ebx

    movl %ebp, %esp
    popl %ebp
    ret


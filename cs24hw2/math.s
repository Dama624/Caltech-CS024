/* This file contains IA32 assembly-language implementations of three
 * basic, very common math operations.
 */

    .text

/*====================================================================
 * int f1(int x, int y)
 */
.globl f1
f1:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	cmpl	%edx, %eax
	cmovg	%edx, %eax
	popl	%ebp
	ret


/*====================================================================
 * int f2(int x)
 */
.globl f2
f2:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	xorl	%edx, %eax
	subl	%edx, %eax
	popl	%ebp
	ret


/*====================================================================
 * int f3(int x)
 */
.globl f3
f3:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sarl	$31, %eax
	testl	%edx, %edx
	movl	$1, %edx
	cmovg	%edx, %eax
	popl	%ebp
	ret


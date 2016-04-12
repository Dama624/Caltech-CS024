.globl gcd	                 /* Ignore everything beginning with a "." */

gcd:
	pushl %ebp				 /* Push old base pointer. */
	movl %esp,%ebp           /* Current stack is new base. */
	cmpl $0,12(%ebp)         /* See if b == 0 */
	je gcd_base              /* If so, move to base case */
gcd_recursion:
	movl 8(%ebp),%eax        /* Move a */
	xor %edx,%edx            /* Zero out %edx */
	idivl 12(%ebp)           /* Divide a by b */
	push %edx                /* Store b = a mod b */
	movl 12(%ebp),%eax       /* a = b */
	push %eax                /* Store a = b */
	call gcd                 /* Recurse */
	jmp gcd_return

gcd_base:
	movl 8(%ebp),%eax        /* Return a */
gcd_return:
	movl %ebp,%esp           /* Pop local stack. */
	popl %ebp                /* Pop old base of frame. */
	ret

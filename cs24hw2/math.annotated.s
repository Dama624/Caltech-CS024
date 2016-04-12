/* This file contains IA32 assembly-language implementations of three
 * basic, very common math operations.
 *
 *
 * The "common theme" between the three implementations is the complete
 * avoidance of the use of conditional statements. There are no jump
 * operations. The implementations here take advantage of conditional
 * expressions that perform both the comparison and the operation in the same
 * line. In the jump implementation, the processor use branch prediction
 * logic to predict which jump instruction is followed; however, in incorrect
 * guesses, all work done in that branch must be discarded, effectively
 * wasting time.
 */

    .text

/*====================================================================
 * int f1(int x, int y)
 * 
 * Normally returns y. If y > x, the program returns x instead. 
 * The program basically returns the lesser value between x and y.
 */
.globl f1
f1:
	pushl	%ebp // Establish the base pointer
	movl	%esp, %ebp
	movl	8(%ebp), %edx // x -> %edx
	movl	12(%ebp), %eax // y -> %eax
	cmpl	%edx, %eax // Compare y to x
	cmovg	%edx, %eax // if y > x, y = x
	popl	%ebp
	ret


/*====================================================================
 * int f2(int x)
 *
 * Returns the absolute value of x. If y > 0, the sign bit is 0. After
 * shifting 31 bits, y = 0. 0 XOR x = x, and x - 0 = x.
 * If y < 0, the sign bit is 1. After shifting, y bits are all 1. y XOR x
 * converts all of x 0 bits to 1 and vice versa (effectively a NOT op). Since
 * 0xFF...F = -1, subtracting -1 is the same as adding 1. Thus for negative
 * numbers, the end result is (NOT x) + 1 which is the same as -x in twos
 * complement.
 */
.globl f2
f2:
	pushl	%ebp // Establish the base pointer
	movl	%esp, %ebp
	movl	8(%ebp), %eax // x -> %eax
	movl	%eax, %edx // "y" = x -> %edx
	sarl	$31, %edx // y >> 31 (arithmetic); 0 if > 0, -1 if < 0
	xorl	%edx, %eax // x = y XOR x
	subl	%edx, %eax // x = x - y
	popl	%ebp
	ret


/*====================================================================
 * int f3(int x)
 * 
 * Returns 1 if x is not 0, and returns 0 if x = 0. Simply put, returns
 * whether x is nonzero. If x is nonzero, x & x does not set the zero flag,
 * and thus %eax stores the value 1. If x is zero, testl sets the zero flag,
 * preventing the cmovg operation from occurring. Since x is zero in this
 * case, y >> 31 must also equal 0 (otherwise it would be -1). Since %eax
 * stores y before the cmovg operation, when x = 0, the function returns 0. 
 */
.globl f3
f3:
	pushl	%ebp // Establish the base pointer
	movl	%esp, %ebp
	movl	8(%ebp), %edx // x -> %edx
	movl	%edx, %eax // "y" = x -> %eax
	sarl	$31, %eax // y >> 31 (arithmetic)
	testl	%edx, %edx // x & x; sets ZF if x = 0
	movl	$1, %edx // 1 -> x
	cmovg	%edx, %eax // if (x & x), y = 1
	popl	%ebp
	ret


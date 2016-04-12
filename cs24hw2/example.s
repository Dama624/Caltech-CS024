	.file	"example.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	ex
	.type	ex, @function
ex:
.LFB0:
	.cfi_startproc
	movl	8(%esp), %eax
	subl	12(%esp), %eax
	imull	4(%esp), %eax
	addl	16(%esp), %eax
	ret
	.cfi_endproc
.LFE0:
	.size	ex, .-ex
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.ident	"GCC: (GNU) 4.9.2 20150304 (prerelease)"
	.section	.note.GNU-stack,"",@progbits

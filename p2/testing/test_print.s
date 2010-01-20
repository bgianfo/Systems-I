	.file	"test_print.c"
	.version	"01.01"
.globl root
	.data
	.align 4
	.type	root, @object
	.size	root, 20
root:
	.long	tdn
	.long	st1
	.zero	12
	.section	.rodata
.LC0:
	.string	"3 Dog Night"
.LC1:
	.string	"The Best of 3 Dog Night"
.globl tdn
	.data
	.align 4
	.type	tdn, @object
	.size	tdn, 20
tdn:
	.long	cre
	.long	reo
	.long	.LC0
	.long	.LC1
	.value	4134
	.byte	20
	.zero	1
	.section	.rodata
.LC2:
	.string	"Cream"
	.align 4
.LC3:
	.string	"Strange Brew - The Very Best of Cream"
.globl cre
	.data
	.align 4
	.type	cre, @object
	.size	cre, 20
cre:
	.long	csn
	.long	tdn
	.long	.LC2
	.long	.LC3
	.value	2632
	.byte	12
	.zero	1
	.section	.rodata
.LC4:
	.string	"Crosby, Stills & Nash"
.globl csn
	.data
	.align 4
	.type	csn, @object
	.size	csn, 20
csn:
	.long	iro
	.long	st2
	.long	.LC4
	.long	.LC4
	.value	2440
	.byte	10
	.zero	1
	.section	.rodata
.LC5:
	.string	"Iron Butterfly"
.LC6:
	.string	"In-A-Gadda-Da-Vida"
.globl iro
	.data
	.align 4
	.type	iro, @object
	.size	iro, 20
iro:
	.long	lnm
	.long	lnm
	.long	.LC5
	.long	.LC6
	.value	3488
	.byte	8
	.zero	1
	.section	.rodata
.LC7:
	.string	"Loggins & Messina"
.LC8:
	.string	"Mother Lode"
.globl lnm
	.data
	.align 4
	.type	lnm, @object
	.size	lnm, 20
lnm:
	.long	reo
	.long	cre
	.long	.LC7
	.long	.LC8
	.value	2665
	.byte	10
	.zero	1
	.section	.rodata
.LC9:
	.string	"REO Speedwagon"
.LC10:
	.string	"The Hits"
.globl reo
	.data
	.align 4
	.type	reo, @object
	.size	reo, 20
reo:
	.long	st1
	.long	0
	.long	.LC9
	.long	.LC10
	.value	3642
	.byte	14
	.zero	1
	.section	.rodata
.LC11:
	.string	"Styx"
.LC12:
	.string	"Cornerstone"
.globl st1
	.data
	.align 4
	.type	st1, @object
	.size	st1, 20
st1:
	.long	st2
	.long	csn
	.long	.LC11
	.long	.LC12
	.value	2317
	.byte	8
	.zero	1
	.section	.rodata
.LC13:
	.string	"Greatest Hits"
.globl st2
	.data
	.align 4
	.type	st2, @object
	.size	st2, 20
st2:
	.long	vog
	.long	vog
	.long	.LC11
	.long	.LC13
	.value	4521
	.byte	16
	.zero	1
	.section	.rodata
.LC14:
	.string	"Vogues, The"
.globl vog
	.data
	.align 4
	.type	vog, @object
	.size	vog, 20
vog:
	.long	0
	.long	iro
	.long	.LC14
	.long	.LC13
	.value	3087
	.byte	18
	.zero	1
	.section	.rodata
.LC15:
	.string	"%-40s %-25s %3i %02i:%02i\n"
	.text
.globl print
	.type	print, @function
print:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	leal	-4(%ebp), %eax

	addl	$20, (%eax)
	leal	-4(%ebp), %eax
	subl	$20, (%eax)
	
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -8(%ebp)
	subl	$8, %esp

	pushl	$0
	pushl	$0
	movl	-8(%ebp), %eax
	movzbl	18(%eax), %eax
	pushl	%eax
	movl	-8(%ebp), %eax
	pushl	8(%eax)
	movl	-8(%ebp), %eax
	pushl	12(%eax)
	pushl	$.LC15
	call	printf
	addl	$32, %esp
	leave
	ret
	.size	print, .-print
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	addl	$15, %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	subl	%eax, %esp
	subl	$4, %esp
	pushl	$0
	pushl	$4
	pushl	$root
	call	print
	addl	$16, %esp
	decl	__iob+16
	cmpl	$0, __iob+16
	jns	.L3
	subl	$8, %esp
	pushl	$__iob+16
	pushl	$10
	call	__flsbuf
	addl	$16, %esp
	jmp	.L4
.L3:
	movl	__iob+20, %eax
	movb	$10, (%eax)
	incl	__iob+20
.L4:
	subl	$4, %esp
	pushl	$0
	pushl	$5
	pushl	$root
	call	print
	addl	$16, %esp
	decl	__iob+16
	cmpl	$0, __iob+16
	jns	.L5
	subl	$8, %esp
	pushl	$__iob+16
	pushl	$10
	call	__flsbuf
	addl	$16, %esp
	jmp	.L6
.L5:
	movl	__iob+20, %eax
	movb	$10, (%eax)
	incl	__iob+20
.L6:
	subl	$4, %esp
	pushl	$.LC11
	pushl	$0
	pushl	$root
	call	print
	addl	$16, %esp
	decl	__iob+16
	cmpl	$0, __iob+16
	jns	.L7
	subl	$8, %esp
	pushl	$__iob+16
	pushl	$10
	call	__flsbuf
	addl	$16, %esp
	jmp	.L8
.L7:
	movl	__iob+20, %eax
	movb	$10, (%eax)
	incl	__iob+20
.L8:
	subl	$4, %esp
	pushl	$.LC13
	pushl	$1
	pushl	$root
	call	print
	addl	$16, %esp
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.4.5"

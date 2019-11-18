# A translation of <my?/Duanes?> version of chase() and moveM.
# (c) 2015 <your name here>

# first, allocate room in the "data" area of the program
# for a globally accessable array, BDay.  It consists of 3 longs.
	.data
	.align	4  # start on a long-word boundary
	.globl	BDay
# .type and .size gdb a hint as to the meaning of "BDay"
	.type	BDay,@object
BDay:	
	.long	12,15,1960	# your birthday month, day, year
	.size	BDay,.-BDay

# these are constants we might use in the code;
# ripped shamelessly from theseus.h
	.equ	NIBBLE,0xf
	.equ	MINOTAUR_SPEED,2
	.equ	LEFT,1
	.equ	UP,2
	.equ	RIGHT,4
	.equ	DOWN,8
	.equ	MINOTAUR,32
	
# standard prolog for functions we write:
	.text
	.globl	chase
	.type	chase,@function
chase:
	pushq	%rbx		# level
	pushq	%r12		# s
	pushq	%r13		# i
	pushq	%r14		# moves
	movq	%rdi,%rbx
	movl	%esi,%r12d
	movl	$0,%r14d
	movl	$0,%r13d
loop:	cmpl	%r12d,%r13d
	jge	overloop
	movq	%rbx,%rdi
	call	moveM
	addl	%eax,%r14d
	addl	$1,%r13d
	jmp	loop
overloop:	
	movl	%r14d,%eax		# return 0
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	ret
# this computes the size of chase.  "." is the current address
# this is a help to the debugger (should you need it)
	.size	chase,.-chase

	.text
	.globl	moveM
	.type	moveM,@function
# registers used:
# variable	register
# level		%rdi
# mch		%sil
# tr		%ebx
# tc		%r12d
# mr		%r13d
# mc		%r14d
# result	%r15d
moveM:
	pushq	%rbx
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15
	pushq	%rdi
	
	popq	%rdi
	pushq	%rdi
	call	getT
	movl	%eax,%ebx
	movl	%ebx,%r12d
	andl	$NIBBLE,%r12d
	shrl	$4,%ebx

	popq	%rdi
	pushq	%rdi
	call	getM
	movl	%eax,%r13d
	movl	%r13d,%r14d
	andl	$NIBBLE,%r14d
	shrl	$4,%r13d

	popq	%rdi
	pushq	%rdi
	movl	%r13d,%esi
	movl	%r14d,%edx
	call	get
	movb	%al,%sil

	# if tc < mc && !(mch & LEFT)
	cmpl	%r14d,%r12d
	jge	if2
	movb	%sil,%al
	andb	$LEFT,%al
	cmpb	$0,%al
	jnz	if2
	
	popq	%rdi
	pushq	%rdi
	movl	$MINOTAUR,%esi
	movl	%r13d,%edx
	movl	%r14d,%ecx
	movl	%r13d,%r8d
	movl	%r14d,%r9d
	subl	$1,%r9d
	call	movePiece
	jmp	overif

if2:		# if tc > mc && !(mch & RIGHT)
	cmpl	%r14d,%r12d
	jle	if3
	movb	%sil,%al
	andb	$RIGHT,%al
	cmpb	$0,%al
	jnz	if3
	
	popq	%rdi
	pushq	%rdi
	movl	$MINOTAUR,%esi
	movl	%r13d,%edx
	movl	%r14d,%ecx
	movl	%r13d,%r8d
	movl	%r14d,%r9d
	addl	$1,%r9d
	call	movePiece
	jmp	overif

if3:		# if tr < mr && !(mch & UP)
	cmpl	%r13d,%ebx
	jge	if4
	movb	%sil,%al
	andb	$UP,%al
	cmpb	$0,%al
	jnz	if4
	
	popq	%rdi
	pushq	%rdi
	movl	$MINOTAUR,%esi
	movl	%r13d,%edx
	movl	%r14d,%ecx
	movl	%r13d,%r8d
	subl	$1,%r8d
	movl	%r14d,%r9d
	call	movePiece
	jmp	overif

if4:	# if tr > mr && !(mch & DOWN)
	cmpl	%r13d,%ebx
	jle	if5
	movb	%sil,%al
	andb	$DOWN,%al
	cmpb	$0,%al
	jnz	if5
	
	popq	%rdi
	pushq	%rdi
	movl	$MINOTAUR,%esi
	movl	%r13d,%edx
	movl	%r14d,%ecx
	movl	%r13d,%r8d
	addl	$1,%r8d
	movl	%r14d,%r9d
	call	movePiece
	jmp	overif
if5:	movl	$0,%r15d
overif:	movl	%r15d,%eax
	popq	%rdi
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	ret
	.size	moveM,.-moveM

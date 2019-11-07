# draw a pattern of gcd's.
	.text
	.globl	pattern
	.type	pattern,@function
# File pointer f (quad) appears in rdi, put in rbx
# range (int/long/doubleword) appears in esi, put in r12d
# row: r13d
# col: r14d
# rp:  r15d
pattern:
	# first, save the registers r13-r15 ; we always save quad words
	pushq	%rbx
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15
	# move parameters into local temporaries
	# (necessary because we're calling other routines)
	movq	%rdi,%rbx	# save file pointer, f
	movl	%esi,%r12d	# save range
	movl	$1,%r13d	# row = 1
oloop:	cmpl	%r12d,%r13d	# row ?? range
	jg	endoloop	# row > range - leave loop
	movl	$1,%r14d	# col = 1
iloop:	cmpl	%r13d,%r14d	# col ?? row
	jg	endiloop	# col > row - leave loop
	# call gcd
	movl	%r13d,%edi	# first param: row
	movl	%r14d,%esi	# second param: col
	call	gcd		# gcd(row,col)
	movl	%eax,%r15d	# save result in rp
	# fputc ' '
	movl	$32,%edi	# 32 is ascii for space
	movq	%rbx,%rsi	# f is second param
	call	fputc
	cmpl	$1,%r15d	# rp ?? 1
	jne	elsepart	# rp != 1, go to else
	# fputc '+'
	movl	$'+,%edi	# ascii char has *one* quote
	movq	%rbx,%rsi	# f is second param
	call	fputc		
	jmp	overif
elsepart:	
	# fputc '*'
	movl	$'*,%edi	# ascii char has *one* quote
	movq	%rbx,%rsi	# f is second param
	call	fputc		
overif:	
	incl	%r14d		# col++
	jmp	iloop
endiloop:	
	# fputc '\n'
	movl	$0xA,%edi	# 0xA is hex for ascii newline
	movq	%rbx,%rsi	# f is second param
	call	fputc		
	incl	%r13d		# row++
	jmp	oloop		
endoloop:	
	# restore saved registers *in reverse order*
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	ret

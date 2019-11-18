# A translation of isWordC from ladder.c (appears as isWordAsm)
# (c) 2019 Duane Bailey & <YOUR NAME HERE>

# Students: look for and remove ?? in the following code.
# Dict equates (size is ?? bytes)
	.equ	dict,0 # ??
	.equ	alloced,0 # ??
	.equ	count,0 # ??
	
# Entry equates (entry size is ?? bytes)
	.equ	word,0 # ??
	.equ	que,0 # ??
	.equ	ladd,0 # ??
	.equ	seen,0 # ??
	
	.text
	.globl	isWordAsm
	.type	isWordAsm,@function

# Frame layout/equates (size is 48)
	.equ	ans,-48   # entry */quad
	.equ	ent,-40   # entry */quad
	.equ	dif,-32   # int/long
	.equ	mid,-28   # int/long
	.equ	hi,-24    # int/long
	.equ	lo,-20    # int/long
	.equ	dtn,-16   # dict */quad
	.equ	w,-8      # char */quad
# oldfp	 +0
# return +8
	
isWordAsm:
	# Prolog ??
	
	# Stash parameters w and dtn in call frame
	movq	%rdi,w(%rbp)	# parameter w
	movq	%rsi,dtn(%rbp)	# parameter dtn

	# // remaining candidates
	movl	$0,lo(%rbp)	# lo = 0
	movq	dtn(%rbp),%rax	# dtn...
	movl	count(%rax),%eax  # dtn->count...
	subl	$1,%eax		# ...dtn->count-1
	movl	%eax,hi(%rbp)	# hi = dtn->count-1
	# // ans: entry if found; else 0/null
	movq	$0,ans(%rbp)	# ans = 0

	# while loop:
loop:	# lo <= hi ??
	jg	fini		# while (lo <= hi) ...

	movl	lo(%rbp),%eax	# mid = (lo+hi)/2
	addl	hi(%rbp),%eax
	sarl	$1,%eax
	movl	%eax,mid(%rbp)

	# ent = dtn->dict+mid ??

	movq	w(%rbp),%rdi	# dif = strcmp(w,ent->word)
	movq	ent(%rbp),%rsi
	movq	word(%rsi),%rsi
	call	strcmp
	movl	%eax,dif(%rbp)

	cmpl	$0,dif(%rbp)	# if dif < 0
	jge	elif
	
	movl	mid(%rbp),%eax	# hi = mid-1
	subl	$1,%eax
	movl	%eax,hi(%rbp)
	
	jmp	overif
	
elif:	cmpl	$0,dif(%rbp)
	jle	finalelse
	
	movl	mid(%rbp),%eax	# lo = mid+1
	addl	$1,%eax
	movl	%eax,lo(%rbp)
	
	jmp	overif

finalelse:
	movq	ent(%rbp),%rax
	movq	%rax,ans(%rbp)
	jmp	fini

overif:	jmp	loop

fini:	movq	ans(%rbp),%rax
	# epilogue ??
	ret
	.size	isWordAsm,.-isWordAsm

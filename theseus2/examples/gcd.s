# A little function to compute greatest common divisor.
        .text
        .globl  gcd
        .type   gcd,@function
# The parameter a will be in edi.
# The parameter b will be in esi.
# The local variable, result, (only live after calls) we will store in eax
gcd:
        cmpl    $0,%edi         # (a ?? 0)
        jne     else1           # if a != 0, jump over then code
        movl    %esi,%eax       # result = b
        jmp     fini            # go to single point of return
else1:  cmpl    %esi,%edi       # (a ?? b)
        jle     else2           # if a <= b, jump over then code
        xchgl   %esi,%edi       # swaps first and second parameters!! :-)
        call    gcd             # recursive call
        jmp     fini            # result in eax
else2:  subl    %edi,%esi       # second parameter decremented by first
        call    gcd             # recursive call
fini:   ret                     # return, result in eax
        .size   gcd,.-gcd

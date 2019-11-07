global start
section .text
start:
  mov eax, 0x2000004 ; write
  mov edi, 1 ; stdout
  mov esi, msg
  mov edx, msg.len
  syscall
  mov eax, 0x2000001 ; exit
  mov edi, 0
  syscall


section .data
msg:    db      "Hello, world!", 10
.len:   equ     $ - msg

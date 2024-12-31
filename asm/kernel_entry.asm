[bits 32]
[extern kernel_main]    ; kernel.c

section .text          
global _start
_start:
    mov esp, kernel_stack_top ; Set up stack.
    
     
    push 0  ; Clear the EFLAGS (Whatever that is.)
    popf
    
    
    call kernel_main ; Call kernel_main func.
    cli ; If kernel returns, halt
    hlt

section .bss
align 4
kernel_stack_bottom:
    resb 16384  ; 16 KB for stack, good enough for now.
kernel_stack_top:

[bits 32]
[extern kernel_main]    ; Declare kernel_main as external

section .text          ; Fix the typo from .txt to .text
global _start
_start:
    ; Set up stack properly
    mov esp, kernel_stack_top
    
    ; Clear EFLAGS
    push 0
    popf
    
    ; Call kernel_main
    call kernel_main
    
    ; If kernel returns, halt
    cli
    hlt

section .bss
align 4
kernel_stack_bottom:
    resb 16384  ; 16 KB for stack
kernel_stack_top:

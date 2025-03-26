[bits 64] ; Wicks, 2025: Rewritten for 64 bit architecture.
[extern kernel_main]    

section .text                ; This section is for the executable code of your kernel.
global _start
_start:
    mov rsp, kernel_stack_top ; Set up the stack pointer (rsp) to the top of our kernel stack. 
    ; Clear the RFLAGS register.
    push 0
    popfq ; 

    ; Call the kernel_main function (kernel.c).
    call kernel_main

    ; If kernel_main were to return, we would disable interrupts and halt the processor.
    cli ;
    hlt ; 

section .bss
align 8 ; Align the following data on an 8-byte boundary (good for 64-bit).
kernel_stack_bottom:
    resb 65536      ; Reserve 65536 bytes (64 KB) of memory for the kernel stack
kernel_stack_top:    ; This label marks the top of the kernel stack (stack grows downwards).

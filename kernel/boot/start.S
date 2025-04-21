
# Stage 1 bootloader for AuroraOS, inspiration taken from Tetris OS.

.code32                    
.section .text.prologue    

.global _start            # Entry point.
_start:
    movl $stack, %esp      # Stack
    andl $-16, %esp        # Align stack.
    movl $0xDEADBEEF, %eax 
    pushl %esp            
    pushl %eax             
    cli                     
    call _main              # Call main loop. kernel/main.c

.section .text            
.align 4                   

.global idt_load           
.type idt_load, @function  
idt_load:
    mov 4(%esp), %eax      
    lidt (%eax)
    ret                      # Return.

.macro ISR_NO_ERR index   
    .global _isr\index    
    _isr\index:           .
        cli                  
        push $0              
        push $\index         
        jmp isr_common
.endm

.macro ISR_ERR index   
    .global _isr\index    
    _isr\index:           
        cli             
        push $\index      
        jmp isr_common    
.endm

ISR_NO_ERR 0             # Div by zero.
ISR_NO_ERR 1             # Single step.
ISR_NO_ERR 2             # NMI.
ISR_NO_ERR 3             # Breakpoint.
ISR_NO_ERR 4             # Overflow.
ISR_NO_ERR 5             # Bound range.
ISR_NO_ERR 6             # Bad opcode.
ISR_NO_ERR 7             # No device.
ISR_ERR 8              # Double fault (err).
ISR_NO_ERR 9             # Coprocessor.
ISR_ERR 10             # Bad TSS (err).
ISR_ERR 11             # No segment (err).
ISR_ERR 12             # Stack fault (err).
ISR_ERR 13             # GP fault (err).
ISR_ERR 14             # Page fault (err).
ISR_NO_ERR 15            # Reserved.
ISR_NO_ERR 16            # FPU error.
ISR_NO_ERR 17            # Align check.
ISR_NO_ERR 18            # Machine check.
ISR_NO_ERR 19            # SIMD FP.
ISR_NO_ERR 20            # Virtual.
ISR_NO_ERR 21            # Control prot.
ISR_NO_ERR 22            # Reserved.
ISR_NO_ERR 23            # Reserved.
ISR_NO_ERR 24            # Reserved.
ISR_NO_ERR 25            # Reserved.
ISR_NO_ERR 26            # Reserved.
ISR_NO_ERR 27            # Reserved.
ISR_NO_ERR 28            # Hypervisor.
ISR_NO_ERR 29            # VMM comm.
ISR_NO_ERR 30            # Security.
ISR_NO_ERR 31            # Reserved.
ISR_NO_ERR 32            # IRQ0 (timer).
ISR_NO_ERR 33            # IRQ1 (kbd).
ISR_NO_ERR 34            # IRQ2 (cascade).
ISR_NO_ERR 35            # IRQ3 (com2).
ISR_NO_ERR 36            # IRQ4 (com1).
ISR_NO_ERR 37            # IRQ5 (lpt2).
ISR_NO_ERR 38            # IRQ6 (floppy).
ISR_NO_ERR 39            # IRQ7 (lpt1).
ISR_NO_ERR 40            # IRQ8 (rtc).
ISR_NO_ERR 41            # IRQ9 (acpi).
ISR_NO_ERR 42            # IRQ10 (reserved).
ISR_NO_ERR 43            # IRQ11 (reserved).
ISR_NO_ERR 44            # IRQ12 (mouse).
ISR_NO_ERR 45            # IRQ13 (fpu).
ISR_NO_ERR 46            # IRQ14 (ide).
ISR_NO_ERR 47            # IRQ15 (ide2).

/* defined in init/isr.c */
.extern isr_handler      
.type isr_handler, @function 

isr_common:               
    pusha                  # Save regs.
    push %ds                # Save DS.
    push %es                # Save ES.
    push %fs                # Save FS.
    push %gs                # Save GS.

    mov $0x10, %ax        
    mov %ax, %ds            # Set DS.
    mov %ax, %es            # Set ES.
    mov %ax, %fs            # Set FS.
    mov %ax, %gs            # Set GS.
    cld               

    push %esp              
    call isr_handler       
    add $4, %esp            # Clean stack.

    pop %gs                 # Restore GS.
    pop %fs                 # Restore FS.
    pop %es                 # Restore ES.
    pop %ds                 # Restore DS.

    popa                   # Restore regs.

    add $8, %esp          
    iret                   # Return

.section .data             
.align 32                   
stack_begin:                # start.
    .fill 0x4000            # 16KB stack.
stack:                      # end.
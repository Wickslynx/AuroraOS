# Stage 0 bootloader for AuroraOS, inspiration taken from Tetris OS.



.code16             # - 16 bit -

.org 0             

.text             
.global _start       
_start:

   # - segment setup -
    mov %cs, %ax     # Code Segment -> AZX
    mov %ax, %ds     # Data Segment = AZX
    mov %ax, %es     # Extra Segment = AZX
    mov %ax, %fs     # FS Segment = AZX
    mov %ax, %gs     # GS Segment = AZX
    mov %ax, %ss     # Stack Segment = AZX

    # -stack-
    movw $0x3000, %sp  # Stack -> 0x3000

    # - boot drive.
    mov %dl, drive_num # Boot drive number -> drive_num

    sti              

    # Print's Welcome to AuroraOS
    movw $welcome_str, %si 
    call print           

    # - Notes for my memory :sob:
    movw $20, %cx         # How many chunks 
    movb drive_num, %dl    # Which drive
    movw $disk_packet, %si # Where to read
    movw $0x1000, segment  # Segment
    movw $1, sector       

sector_loop:
    movb $0x42, %ah     #  Calls main.

    int $0x13           
    jc disk_error       # ERROR
    addw $64, sector      
    jnc sector_same_segment

   
    addw $0x1000, segment # Next chunk
    movw $0x0000, offset  # Back to start

sector_same_segment:
    loop sector_loop      

    # TODO: Use modern color modes.
    movb $0x00, %ah     
    movb $0x13, %al     
    int $0x10           

    # Enable all ram
    cli               

   
    call enable_a20_wait0
    movb $0xD0, %al
    outb $0x64
    call enable_a20_wait1
    xorw %ax, %ax
    inb $0x60

    pushw %ax
    call enable_a20_wait0
    movb $0xD1, %al
    outb $0x64
    call enable_a20_wait0
    popw %ax
    orw $0x2, %ax
    outb $0x60

    # - protected mode -
    movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0

    # - jump -
    jmp flush

.code32             # - 32 bit -

flush:
    lidt idt         
    lgdt gdtp         
    # - data memory views -
    movw $(gdt_data_segment - gdt_start), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movl $0x3000, %esp # 32 bit stack

    # - jmp to start off loaded kernel -
    ljmp $0x8, $entry32

entry32:
    # - run kernel -
    movl $0x10000, %eax
    jmpl *%eax

_loop:
    jmp _loop          # if we get back here somehow js run forever.

.code16             # 16 bit for all of these helpers.

enable_a20_wait0:
    xorw %ax, %ax
    inb $0x64
    btw $1, %ax
    jc enable_a20_wait0
    ret

enable_a20_wait1:
    xorw %ax, %ax
    inb $0x64
    btw $0, %ax
    jnc enable_a20_wait1
    ret

disk_error:
    movw $disk_error_str, %si 
    call print            # show error

# - PRINT TEXT ON SCREEN -
print:
    xorb %bh, %bh    
    movb $0x0E, %ah     

print_char:
    lodsb             
    cmpb $0, %al      
    je print_end     

    int $0x10         
    jmp print_char 

print_end:
    ret

# - MESSAGES -
welcome_str:
    .asciz "Welcome to AuroraOS!\n"
disk_error_str:
    .asciz "DISK ERROR\n"

# - boot drive num-
drive_num:
    .word 0x0000

# disk info
disk_packet:
    .byte 0x10
    .byte 0x00
num_sectors:
    .word 0x0040
offset:
    .word 0x0000
segment:
    .word 0x0000
sector:
    .quad 0x00000000

# Memory map smth  (GDT)
.align 16
gdtp:
    .word gdt_end - gdt_start - 1
    .long gdt_start

.align 16
gdt_start:
gdt_null:
    .quad 0
gdt_code_segment:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x00
gdt_data_segment:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x00
gdt_end:

# Interrupt map thingy (IDT) - Empty for now
idt:
    .word 0
    .long 0

# boot signature (bootable)
.fill 510-(.-_start), 1, 0
.word 0xAA55
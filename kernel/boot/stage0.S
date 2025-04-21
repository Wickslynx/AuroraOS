.code16                      # 16-bit real mode
.org 0                       # Start at 0x0000

.text
.global _start
_start:
    mov %cs, %ax             # Segment setup
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    movw $0x3000, %sp        # Stack setup
    mov %dl, drive_num       # Boot drive
    sti                      # Enable interrupts
    movw $welcome_str, %si    # Print welcome
    call print
    movw $20, %cx             # Kernel load loop
    movb drive_num, %dl
    movw $disk_packet, %si
    movw $0x1000, segment
    movw $1, sector
sector_loop:
    movb $0x42, %ah           # BIOS read
    int $0x13
    jc disk_error            # Error check
    addw $64, sector
    jnc sector_same_segment
    addw $0x1000, segment
    movw $0x0000, offset
sector_same_segment:
    loop sector_loop
    movb $0x00, %ah           # Graphics mode
    movb $0x13, %al
    int $0x10
    cli                      # Disable interrupts
    call enable_a20_wait0    # Enable A20
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
    movl %cr0, %eax           # Protected mode enable
    orl $0x1, %eax
    movl %eax, %cr0
    jmp flush                # Jump to 32-bit

.code32                      # 32-bit protected mode

flush:
    lidt idt_ptr             # IDT setup
    lgdt gdtp                # GDT setup
    movw $(gdt_data_segment - gdt_start), %ax # Data segments
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movl $0x3000, %esp        # Stack setup
    ljmp $0x8, $entry32       # Long jump to kernel

entry32:
    movl $0x10000, %eax       # Kernel start address
    jmpl *%eax              # Jump to kernel

_loop:
    jmp _loop                # Kernel return loop

.code16                      # 16-bit helpers

enable_a20_wait0:           # A20 enable
    xorw %ax, %ax
    inb $0x64
    btw $1, %ax
    jc enable_a20_wait0
    ret

enable_a20_wait1:           # A20 enable
    xorw %ax, %ax
    inb $0x64
    btw $0, %ax
    jnc enable_a20_wait1
    ret

disk_error:                 # Disk error handler
    movw $disk_error_str, %si
    call print

print:                      # Print string
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

welcome_str:                # Messages
    .asciz "Welcome to AuroraOS!\n"
disk_error_str:
    .asciz "DISK ERROR\n"

drive_num:                  # Boot drive storage
    .word 0x0000

disk_packet:                # Disk transfer struct
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

.align 16                   # GDT
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

idt_ptr:                    # IDT
    .word idt_end - idt - 1
    .long idt
idt:
    .rept 256
        .quad 0
    .endr
idt_end:

.fill 510-(.-_start), 1, 0  # Boot signature
.word 0xAA55
.code16
.org 0



.text

.global _start
_start:
    cli

    /* init segment registers */
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    /* init stack pointer */
    movw $0x3000, %sp

    /* save drive num*/
    mov %dl, drive_number

    sti

    /* Display "Booting AuroraOS..." */
    movw $welcome_message, %si
    call print_string

    /* load kernel from disk */
    movw $32, %cx         /* (32 * num_sectors) */
    movb drive_number, %dl
    movw $disk_address_packet, %si
    movw $0x1000, kernel_segment
    movw $1, kernel_sector

    pushw %si
    /* Display "Loading kernel..." */
    movw $kernel_load_message, %si
    call print_string
    popw %si   

load_kernel_loop:
    movb $0x42, %ah     /* extended read */
    int $0x13
    jc disk_read_error
  

    addw $64, kernel_sector
    addw $0x8000, kernel_offset
    jnc same_segment


    /* +segment, reset offset */
    addw $0x1000, kernel_segment
    movw $0x0000, kernel_offset

same_segment: 
    loop load_kernel_loop

    /* Display "Setting video mode..." */
    movw $kernel_set_video_message, %si
    call print_string

    /* set video mode: 320x200 */
    movb $0x00, %ah
    movb $0x13, %al
    int $0x10

    cli

    /*save cstate */
    call enable_a20_wait_clear
    movb $0xD0, %al
    outb $0x64
    call enable_a20_wait_set
    xorw %ax, %ax
    inb $0x60

    /* new state*/
    pushw %ax
    call enable_a20_wait_clear
    movb $0xD1, %al
    outb $0x64
    call enable_a20_wait_clear
    popw %ax
    orw $0x2, %ax
    outb $0x60

    /* protected mode */
    movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0

    /* flush pipeline */
    jmp protected_mode_entry

protected_mode_entry:
    /* Display "Entering protected mode..." */
    movw $protected_mode_message, %si
    call print_string

    lidt interrupt_descriptor_table_pointer
    lgdt global_descriptor_table_pointer

    movw $(DATA_SEGMENT - GLOBAL_DESCRIPTOR_TABLE_START), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movl $0x3000, %esp
    ljmp $0x8, $kernel_entry_point

.code32
kernel_entry_point:
    /* jump to kernel*/
    movl $0x10000, %eax
    jmpl *%eax

infinite_loop:
    jmp infinite_loop

.code16
enable_a20_wait_clear:
    xorw %ax, %ax
    inb $0x64
    testb $0x02, %al
    jnz enable_a20_wait_clear
    ret

enable_a20_wait_set:
    xorw %ax, %ax
    inb $0x64
    testb $0x01, %al
    jz enable_a20_wait_set
    ret

disk_read_error:
    pushw %ax           
    movw $disk_error_message, %si
    call print_string

    movb %ah, %al     /* BIOS return status */
    call print_hex  
    popw %ax            
    movb %ah, %al       
    call print_hex      
    cli
    hlt

disk_error_loop:
    jmp disk_error_loop 

/* -- printing functions --- */
print_string:
    xorw %bx, %bx      
    movb $0x0E, %ah    
print_char:
    lodsb
    cmpb $0, %al
    je print_string_done
    int $0x10
    jmp print_char

print_string_done:
    ret

print_hex:
    pusha
    mov %al, %bl
    
    mov %al, %cl
    shr $4, %cl
    call print_hex_digit
    
    mov %bl, %cl
    and $0x0F, %cl
    call print_hex_digit
    
    popa
    ret

print_hex_digit:
    mov $0x0E, %ah
    cmp $10, %cl
    jl print_hex_digit_number
    add $55, %cl
    jmp print_hex_digit_output
print_hex_digit_number:
    add $48, %cl
print_hex_digit_output:
    mov %cl, %al
    int $0x10
    ret

welcome_message:
    .asciz "Booting AuroraOS....\n"
disk_error_message:
    .asciz "DISK ERROR\n"
protected_mode_message:
    .asciz "Entering protected mode...\n"
kernel_set_video_message:
    .asciz "Setting video mode...\n"
kernel_load_message:
    .asciz "Loading kernel...\n"


/* Boot drive number */
drive_number:
    .word 0x0000

/* Disk address packet for INT 13h */
disk_address_packet:
    .byte 0x10             /* size */
    .byte 0x00             /* reserved */
num_kernel_sectors:
    .word 0x0040           /* num of sectors. NOTE: This might be a problem in the future, dunno */
kernel_offset:
    .word 0x0000           /* offset in segment */
kernel_segment:
    .word 0x0000           /* segment address */
kernel_sector:
    .quad 0x00000000       /* LBA sector */

/* GDT */
.align 16
global_descriptor_table_pointer:
    .word GLOBAL_DESCRIPTOR_TABLE_END - GLOBAL_DESCRIPTOR_TABLE_START - 1
    .long GLOBAL_DESCRIPTOR_TABLE_START

/* IDT pointer */
.align 16
interrupt_descriptor_table_pointer:
    .word IDT_END - IDT_START - 1
    .long IDT_START

/* IDT */
.align 16
IDT_START:
    .word 0
    .long 0
IDT_END:

.align 16
GLOBAL_DESCRIPTOR_TABLE_START:
NULL_DESCRIPTOR:
    .quad 0
CODE_SEGMENT:
    .word 0xffff          /* limit (0-15) */
    .word 0x0000          /* base (0-15) */
    .byte 0x00            /* base (16-23) */
    .byte 0b10011010      /* access byte */
    .byte 0b11001111      /* no idea*/
    .byte 0x00            /* base (24-31) */
DATA_SEGMENT:
    .word 0xffff          /* limit (0-15) */
    .word 0x0000          /* base (0-15) */
    .byte 0x00            /* base (16-23) */
    .byte 0b10010010      /* access byte */
    .byte 0b11001111      /* no idea */
    .byte 0x00            /* base (24-31) */
GLOBAL_DESCRIPTOR_TABLE_END:




/* boot sign */
.fill 510-(.-_start), 1, 0
.word 0xAA55

; === Stage 2 Bootloader === 
; AuroraOS, 2025. (Wicks)

[org 0x8000] ; this matches the address loaded by Stage 1
bits 16

start_stage2:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load kernel from disk (e.g., sector 3+)
    mov ah, 0x02        ; INT 13h - Read Sectors
    mov al, 20          ; Read 20 sectors (adjust size as needed)
    mov ch, 0
    mov cl, 4           ; Start reading from sector 4 (LBA 3)
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov es, 0x1000      ; Load kernel at 0x100000 (segment:offset = 0x1000:0000)
    int 0x13
    jc fail

    ; Setup GDT
    lgdt [gdt_descriptor]

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump into protected mode
    jmp 0x08:protected_mode

; ==== GDT Setup ====
gdt_start:
    dq 0                ; null
gdt_code:
    dw 0xFFFF           ; limit
    dw 0                ; base low
    db 0                ; base middle
    db 10011010b        ; code segment
    db 11001111b        ; granularity
    db 0                ; base high
gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b        ; data segment
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ==== Protected Mode ====
[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000

    ; Enable PAE (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Setup basic page tables here (optional: want to do it manually?)
    ; Enable long mode in MSR
    mov ecx, 0xC0000080     ; IA32_EFER
    rdmsr
    or eax, 1 << 8          ; Set LME bit (Long Mode Enable)
    wrmsr

    ; Enable paging
    mov eax, page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000      ; Set PG bit
    mov cr0, eax

    ; Far jump to long mode
    jmp 0x08:long_mode_entry

; ==== Long Mode Setup ====
[bits 64]
long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0xA0000

    ; Jump to kernel entry point (loaded at 0x100000)
    jmp 0x100000

fail:
    hlt
    jmp $

align 4096
page_directory:
    ; Minimal identity-mapped page table (1GB)
    ; Want help writing this too?

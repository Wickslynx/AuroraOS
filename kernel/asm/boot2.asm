; === Stage 2 Bootloader ===
; AuroraOS, 2025 (Wicks) — High memory support

[org 0x8000]
bits 16

start_stage2:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; === Load KERNEL (sector 4–23) to 0x8000 ===
    mov ah, 0x02
    mov al, 20
    mov ch, 0
    mov cl, 4
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov es, 0x0800       ; 0x0800:0000 = 0x8000
    int 0x13
    jc fail

    ; === Load FILESYSTEM (sector 24–47) to 0xA000 ===
    mov ah, 0x02
    mov al, 24
    mov ch, 0
    mov cl, 24
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov es, 0x0A00       ; 0x0A00:0000 = 0xA000
    int 0x13
    jc fail

    ; === Setup GDT ===
    lgdt [gdt_descriptor]

    ; === Enter Protected Mode ===
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

; === GDT ===
gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; === Enter Protected Mode ===
[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000

    ; Copy kernel to 0x100000
    mov esi, 0x8000
    mov edi, 0x100000
    mov ecx, 512 * 20 / 4
    rep movsd

    ; Copy FS to 0x200000
    mov esi, 0xA000
    mov edi, 0x200000
    mov ecx, 512 * 24 / 4
    rep movsd

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Enable Long Mode (via MSR)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Setup page tables
    mov eax, page_directory
    mov cr3, eax

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    jmp 0x08:long_mode_entry

; === Long Mode ===
[bits 64]
long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0xA0000

    ; Filesystem is at 0x200000, kernel at 0x100000
    jmp 0x100000         ; jump to kernel entry point

fail:
    hlt
    jmp $

; === Identity-Mapped Paging Tables ===
align 4096
page_directory:
    dq page_table + 0x03
    times 511 dq 0

align 4096
page_table:
%assign i 0
%rep 512
    dq i * 0x1000 + 0x03
%assign i i+1
%endrep

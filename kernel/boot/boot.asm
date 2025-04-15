; AuroraOS Stage 1 Bootloader - Based on Tetris-OS Layout
; ===============================

[bits 16]
[org 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, STACK
    jmp 0:main  ; far jump to reload CS

main:
    sti
    mov [BOOT_DRV], dl       ; store boot drive
    call clear_screen
    mov ax, RM_MSG
    call print_cstring
    call load_kernel
    call pm_switch
    hlt

; ===== Constants & Variables =====
KERNEL_OFFSET equ 0x1000
STACK equ 0x9000

BOOT_DRV db 0

; ===== Real Mode Functions =====
%include "pre32_functions/print_char.asm"
%include "pre32_functions/print_cstring.asm"
%include "pre32_functions/clear_screen.asm"
%include "pre32_functions/load_sectors.asm"
%include "pre32_functions/load_kernel.asm"
%include "pre32_functions/pm_switch.asm"
%include "gdt/gdt.asm"
%include "32pm_functions/print_string_pm.asm"

; ===== Protected Mode Entry Point =====
[bits 32]
BEGIN_PM:
    mov eax, PM_LANDED_MSG
    call print_string_pm

    jmp KERNEL_OFFSET

    hlt

; ===== Strings =====
RM_MSG: db "AuroraOS Bootloader: Real mode started.", 0
PM_LANDED_MSG: db "AuroraOS: Protected mode active.", 0

; ===== Boot Sector Padding =====
times 510 - ($ - $$) db 0
dw 0xAA55

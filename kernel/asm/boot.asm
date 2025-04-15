; ==== STAGE 1 BootLoader =====
; AuroraOS, 2025: (Wicks)
[org 0x7C00]
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load 2 sectors from LBA 1 to address 0x8000 (segment:offset = 0x0800:0000)
    mov ah, 0x02        ; INT 13h - Read Sectors
    mov al, 2           ; Read 2 sectors
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Sector 2 (sector count starts at 1, 1 = bootloader)
    mov dh, 0           ; Head 0
    mov dl, 0x80        ; Drive 0x80 (first HDD)
    mov bx, 0x0000      ; Offset
    mov ax, 0x0800
    mov es, ax ; Segment = 0x0800 => 0x0800:0000 = 0x8000
    int 0x13
    jc disk_error

    ; Jump to loaded second-stage loader
    jmp 0x0800:0000

disk_error:
    mov si, error_msg
    call print_string
    hlt

print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

error_msg db 'Disk read error!', 0

times 510 - ($ - $$) db 0
dw 0xAA55

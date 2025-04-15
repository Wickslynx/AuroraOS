; AuroraOS: Load sectors using BIOS CHS mode
[bits 16]
load_sectors:
    ; Expects:
    ; AL = number of sectors
    ; BX = load address (segment offset)
    ; DL = drive
    ; CH = cylinder
    ; CL = sector (1-based)
    ; DH = head

    push ax
    push bx
    push cx
    push dx

.retry:
    mov ah, 0x00        ; Reset disk system
    int 0x13
    jc .retry           ; Retry if error

    mov ah, 0x02        ; Function: Read sectors
    mov es, bx          ; Load address segment (BX is segment)
    xor bx, bx          ; Offset 0x0000 (so ES:BX = full address)
    int 0x13
    jc .fail            ; If error, show message

    ; Check how many sectors were read
    pop cx              ; Restore original requested sectors
    cmp al, cl
    jne .fail

    ; Success
    pop dx
    pop cx
    pop bx
    pop ax
    ret

.fail:
    mov ax, DISK_ERROR
    call print_cstring
    cli
    hlt

DISK_ERROR: db "Disk read error!", 0

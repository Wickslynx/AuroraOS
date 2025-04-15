; AuroraOS: Load Kernel (50 sectors starting from sector 2 into 0x100000)
[bits 16]
load_kernel:
    pusha                   ; Save all general-purpose registers

    mov bx, KERNEL_OFFSET   ; Load address (segment-based, assumes 0x1000 = 0x100000 linear)
    mov al, 50              ; Number of sectors to load
    mov dl, [BOOT_DRV]      ; Boot drive (usually 0x80 for HDD)
    mov ch, 0               ; Cylinder 0
    mov cl, 2               ; Sector 2 (1-based)
    mov dh, 0               ; Head 0

    call load_sectors       ; Perform the read

    popa
    ret

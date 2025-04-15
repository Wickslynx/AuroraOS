; Enhanced bootloader with debugging features
[org 0x8000]
[bits 16]

start_stage2:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; Debug message at bootloader start
    mov si, debug_start
    call print_string
    
    ; ==== Load KERNEL (sector 4-23) to 0x100000 ====
    mov ah, 0x02
    mov al, 20
    mov ch, 0
    mov cl, 4
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov ax, 0x1000
    mov es, ax
    int 0x13
    jc kernel_load_error
    
    mov si, debug_kernel_loaded
    call print_string
    
    ; ==== Load FILESYSTEM (sector 24-47) to 0x200000 ====
    mov ah, 0x02
    mov al, 24
    mov ch, 0
    mov cl, 24
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov ax, 0x2000
    mov es, ax
    int 0x13
    jc fs_load_error
    
    mov si, debug_fs_loaded
    call print_string
    
    ; ==== Setup GDT ====
    lgdt [gdt_descriptor]
    mov si, debug_gdt_loaded
    call print_string
    
    ; ==== Enter Protected Mode ====
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Jump to protected mode setup
    jmp 0x08:protected_mode

kernel_load_error:
    mov si, error_kernel_load
    call print_string
    jmp $

fs_load_error:
    mov si, error_fs_load
    call print_string
    jmp $

; Simple print string function for debugging (16-bit real mode)
print_string:
    push ax
    mov ah, 0x0E        ; BIOS teletype function
.loop:
    lodsb               ; Load character from SI into AL and increment SI
    test al, al         ; Check if character is null (end of string)
    jz .done            ; If null, we're done
    int 0x10            ; Call BIOS interrupt to print character
    jmp .loop           ; Continue with next character
.done:
    pop ax
    ret

; Debug messages
debug_start db "Starting stage 2 bootloader...", 13, 10, 0
debug_kernel_loaded db "Kernel loaded successfully", 13, 10, 0
debug_fs_loaded db "Filesystem loaded successfully", 13, 10, 0
debug_gdt_loaded db "GDT set up successfully", 13, 10, 0
error_kernel_load db "ERROR: Failed to load kernel!", 13, 10, 0
error_fs_load db "ERROR: Failed to load filesystem!", 13, 10, 0

; ==== Protected Mode Setup ====
[bits 32]
protected_mode:
    ; Set up segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000
    
    ; Set up debugging via serial port (COM1)
    mov dx, 0x3F8 + 1  ; COM1 port + line control register offset
    mov al, 0x00       ; Disable interrupts
    out dx, al
    
    mov dx, 0x3F8 + 3  ; Line Control Register
    mov al, 0x80       ; Enable DLAB
    out dx, al
    
    mov dx, 0x3F8      ; Divisor Latch Low Byte
    mov al, 0x01       ; 115200 baud rate
    out dx, al
    
    mov dx, 0x3F8 + 1  ; Divisor Latch High Byte
    mov al, 0x00
    out dx, al
    
    mov dx, 0x3F8 + 3  ; Line Control Register
    mov al, 0x03       ; 8 bits, no parity, 1 stop bit, disable DLAB
    out dx, al
    
    ; Send debug message to serial port
    mov si, debug_protected_mode
    call serial_print
    
    ; Call the improved paging setup function
    call setup_paging
    
    ; Send debug message after paging setup
    mov si, debug_paging_setup
    call serial_print
    
    ; Enable long mode via MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    
    ; Send debug message after long mode enabled
    mov si, debug_long_mode_enabled
    call serial_print
    
    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ; Jump to long mode
    jmp 0x08:long_mode_entry

; Function to send a string to serial port
serial_print:
    push eax
    push edx
.loop:
    mov al, [esi]      ; Load character from ESI
    inc esi            ; Increment ESI to point to next character
    test al, al        ; Check if character is null (end of string)
    jz .done           ; If null, we're done
    
    mov dx, 0x3F8 + 5  ; Line Status Register
.wait:
    in al, dx          ; Read Line Status Register
    test al, 0x20      ; Test if transmitter is empty
    jz .wait           ; If not, wait
    
    mov dx, 0x3F8      ; Data Register
    mov al, [esi-1]    ; Get character to send
    out dx, al         ; Send character
    jmp .loop          ; Continue with next character
.done:
    pop edx
    pop eax
    ret

; Debug messages for protected mode
debug_protected_mode db "Entered protected mode", 13, 10, 0
debug_paging_setup db "Paging setup complete", 13, 10, 0
debug_long_mode_enabled db "Long mode enabled", 13, 10, 0

; ==== Long Mode Setup ====
[bits 64]
long_mode_entry:
    ; Set up segment registers for long mode
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    
    ; Set up stack properly for 64-bit mode
    mov rsp, 0xA0000
    
    ; Send debug message to serial port before jumping to kernel
    mov rdx, 0x3F8 + 5  ; Line Status Register
.wait_tx:
    in al, dx           ; Read Line Status Register
    test al, 0x20       ; Test if transmitter is empty
    jz .wait_tx         ; If not, wait
    
    mov rdx, 0x3F8      ; Data Register
    mov al, 'L'         ; 'L' for "Long mode"
    out dx, al          ; Send character
    
    ; Jump to kernel
    mov rax, 0x100000   ; Kernel entry point
    jmp rax             ; Jump to kernel

#!/bin/bash
# AuroraOS Kernel Build Script

# --- Configuration Paths ---
BOOT_ASM="kernel/asm/boot.asm"
KERNEL_ENTRY_ASM="kernel/asm/kernel_entry.asm"
HOME_C="kernel/os/home.c"
AURO_H="drivers/auro/auro.h"
FILESYS_H="kernel/fs/file_sys.h"     
FILESYS_C="kernel/fs/file_sys.c"       
MACROS_H="utils/macros/macros.h" 
FUNCTIONS_H="utils/funcs/functions.h"                  
LINKER_SCRIPT="linker.ld"

# --- Output Files ---
OUTPUT_BOOT_BIN="boot.bin"
OUTPUT_KERNEL_O="kernel_entry.o"
OUTPUT_FRONTEND_O="home.o"
OUTPUT_FILESYS_O="file_sys.o"
OUTPUT_KERNEL_ELF="kernel.elf"
OUTPUT_KERNEL_BIN="kernel.bin"
OUTPUT_OS_BIN="auroraos.bin"

# --- Compilation Flags ---
NASM_FLAGS="-f elf64"
GCC_FLAGS="-m64 -mno-red-zone -fno-pic -fno-pie -ffreestanding -nostdlib"
INCLUDE_PATHS="-I kernel.h -I frontend -I utils/macros -I utils/funcs -I mem -I init -I fs"
LINKER_FLAGS="-m elf_x86_64 -T $LINKER_SCRIPT"

# --- Error Handling Function ---
handle_error() {
    echo "$1"
    exit 1
}

# --- Assembly Stage ---
echo "Assembling bootloader..."
nasm "$BOOT_ASM" -f bin -o "$OUTPUT_BOOT_BIN" || handle_error "Error assembling bootloader."

echo "Assembling kernel entry..."
nasm "$KERNEL_ENTRY_ASM" $NASM_FLAGS -o "$OUTPUT_KERNEL_O" || handle_error "Error assembling kernel entry."

echo "Compiling kernel main..."
gcc $GCC_FLAGS $INCLUDE_PATHS -c -o "kernel.o" "kernel/kernel.c" || handle_error "Error compiling kernel main."




echo "Compiling frontend..."
gcc $GCC_FLAGS $INCLUDE_PATHS -c -o "$OUTPUT_FRONTEND_O" "$HOME_C" || handle_error "Error compiling frontend."

echo "Compiling file system..."
gcc $GCC_FLAGS $INCLUDE_PATHS -c -o "$OUTPUT_FILESYS_O" "$FILESYS_C" || handle_error "Error compiling file system."


# --- Linking Stage ---
echo "Linking kernel..."
ld $LINKER_FLAGS "$OUTPUT_KERNEL_O" "$OUTPUT_FRONTEND_O" "$OUTPUT_FILESYS_O" "$OUTPUT_MEMORY_O" "kernel.o" -o "$OUTPUT_KERNEL_ELF" || handle_error "Error linking kernel."

echo "Creating binary image..."
objcopy -O binary "$OUTPUT_KERNEL_ELF" "$OUTPUT_KERNEL_BIN" || handle_error "Error creating binary image."

echo "Combining bootloader and kernel..."
cat "$OUTPUT_BOOT_BIN" "$OUTPUT_KERNEL_BIN" > "$OUTPUT_OS_BIN" || handle_error "Error combining bootloader and kernel."

# --- Completion Message ---
echo "Build successful!"
echo "Run AuroraOS with: qemu-system-x86_64 -fda $OUTPUT_OS_BIN"

#!/bin/bash

# This script will build and run your AuroraOS with QEMU.

# --- Configuration ---
BOOT_ASM="asm/boot.asm"
KERNEL_ENTRY_ASM="asm/kernel_entry.asm"
FRONTEND_C="frontend/frontend.c"
FILESYS_H="fs/file_sys.h"     
FILESYS_C="fs/file_sys.c"       
MACROS_H="utils/macros/macros.h" 
FUNCTIONS_H="utils/funcs/functions.h"
MEMORY_C="mem/memory.c"         
MEMORY_H="mem/memory.h"         
INIT_H="init/init.h"            
LINKER_SCRIPT="linker.ld"
OUTPUT_BOOT_BIN="boot.bin"
OUTPUT_KERNEL_O="kernel_entry.o"
OUTPUT_FRONTEND_O="frontend.o"
OUTPUT_FILESYS_O="file_sys.o"   # Object file for file system
OUTPUT_MEMORY_O="memory.o"     # Object file for memory management
OUTPUT_KERNEL_ELF="kernel.elf"
OUTPUT_KERNEL_BIN="kernel.bin"
OUTPUT_OS_BIN="os.bin"

# --- Assembly ---
echo "Assembling bootloader..."
nasm "$BOOT_ASM" -f bin -o "$OUTPUT_BOOT_BIN"
if [ $? -ne 0 ]; then
    echo "Error assembling bootloader. Exiting."
    exit 1
fi

echo "Assembling kernel entry..."
nasm "$KERNEL_ENTRY_ASM" -f elf -o "$OUTPUT_KERNEL_O"
if [ $? -ne 0 ]; then
    echo "Error assembling kernel entry. Exiting."
    exit 1
fi


# --- Compilation ---

echo "Compiling memory management..."
gcc -m32 -c -ffreestanding -nostdlib -I kernel.h -I frontend -I utils/macros -I utils/funcs -I mem -I init -I fs -o "$OUTPUT_MEMORY_O" "$MEMORY_C"
if [ $? -ne 0 ]; then
    echo "Error compiling memory management. Exiting."
    exit 1
fi

echo "Compiling frontend..."
gcc -m32 -c -ffreestanding -nostdlib -I frontend -I utils/macros -I mem -I utils/funcs -I init -I fs -o "$OUTPUT_FRONTEND_O" "$FRONTEND_C"
if [ $? -ne 0 ]; then
    echo "Error compiling frontend. Exiting."
    exit 1
fi

echo "Compiling file system..."
gcc -m32 -c -ffreestanding -nostdlib -I frontend -I utils/macros -I mem -I utils/funcs -I init -I fs -o "$OUTPUT_FILESYS_O" "$FILESYS_C"
if [ $? -ne 0 ]; then
    echo "Error compiling file system. Exiting."
    exit 1
fi



# --- Linking ---
echo "Linking kernel..."
ld -T "$LINKER_SCRIPT" "$OUTPUT_KERNEL_O" "$OUTPUT_FRONTEND_O" "$OUTPUT_FILESYS_O" "$OUTPUT_MEMORY_O" -o "$OUTPUT_KERNEL_ELF"
if [ $? -ne 0 ]; then
    echo "Error linking kernel. Exiting."
    exit 1
fi

echo "Creating binary image..."
objcopy -O binary "$OUTPUT_KERNEL_ELF" "$OUTPUT_KERNEL_BIN"
if [ $? -ne 0 ]; then
    echo "Error creating binary image. Exiting."
    exit 1
fi

echo "Combining bootloader and kernel..."
cat "$OUTPUT_BOOT_BIN" "$OUTPUT_KERNEL_BIN" > "$OUTPUT_OS_BIN"
if [ $? -ne 0 ]; then
    echo "Error combining bootloader and kernel. Exiting."
    exit 1
fi

echo "Done! You can now run AuroraOS with qemu using: qemu-system-i386 -fda $OUTPUT_OS_BIN"

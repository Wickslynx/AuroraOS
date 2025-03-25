#!/bin/bash

# This script will build and run your AuroraOS with QEMU.

# --- Configuration ---
BOOT_ASM="boot.asm"
KERNEL_ENTRY_ASM="kernel_entry.asm"
FRONTEND_C="frontend.c"
LINKER_SCRIPT="linker.ld"
OUTPUT_BOOT_BIN="boot.bin"
OUTPUT_KERNEL_O="kernel_entry.o"
OUTPUT_FRONTEND_O="frontend.o"
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
echo "Compiling frontend..."
gcc -m32 -c -ffreestanding -nostdlib -o "$OUTPUT_FRONTEND_O" "$FRONTEND_C"
if [ $? -ne 0 ]; then
  echo "Error compiling frontend. Exiting."
  exit 1
fi

# --- Linking ---
echo "Linking kernel..."
ld -T "$LINKER_SCRIPT" "$OUTPUT_KERNEL_O" "$OUTPUT_FRONTEND_O" -o "$OUTPUT_KERNEL_ELF"
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

#!/bin/bash

CC=i386-elf-gcc
ASM=i386-elf-as
LD=i386-elf-ld

GFLAGS=""
CCFLAGS="-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing"
CCFLAGS+="-Wno-pointer-arith -Wno-unused-parameter"
CCFLAGS+="-nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector"
CCFLAGS+="-fno-builtin-function -fno-builtin"
ASFLAGS=""
LDFLAGS=""

BOOTSECT_SRCS="kernel/boot/stage0.S"
BOOTSECT_OBJS=$(echo "$BOOTSECT_SRCS" | sed 's/\.S/\.o/g')
KERNEL_C_SRCS=$(find kernel -maxdepth 1 -name "*.c")
KERNEL_ASM_SRCS=$(find kernel -maxdepth 1 -name "*.S")
KERNEL_S_SRCS=$(echo "$KERNEL_ASM_SRCS" | grep -v "$(echo "$BOOTSECT_SRCS" | sed 's/\//\\//g')")
KERNEL_OBJS=$(echo "$KERNEL_C_SRCS" | sed 's/\.c/\.o/g') $(echo "$KERNEL_S_SRCS" | sed 's/\.S/\.o/g')

BOOTSECT="bootsect.bin"
KERNEL="kernel.bin"
ISO="auroraos.iso"

# Create directories
mkdir -p bin

# Clean previous build
rm -rf bin/*.o
rm -rf *.iso
rm -rf bin/*.elf
rm -rf bin/*.bin

# Build bootsect
$(LD) -o "bin/$(BOOTSECT)" "$BOOTSECT_OBJS" -Ttext 0x7C00 --oformat=binary

# Build kernel
$(LD) -o "bin/$(KERNEL)" "$KERNEL_OBJS" $(LDFLAGS) -Tkernel/link.ld

# Compile C sources
for src in $KERNEL_C_SRCS; do
	base=$(basename "$src" .c)
	$(CC) -o "bin/${base}.o" -c "$src" $(GFLAGS) $(CCFLAGS)
done

# Assemble S sources (excluding bootsect)
for src in $KERNEL_S_SRCS; do
	base=$(basename "$src" .S)
	$(ASM) -o "bin/${base}.o" -c "$src" $(GFLAGS) $(ASFLAGS)
done
# Compile bootsect
base_boot=$(basename "$BOOTSECT_SRCS" .S)
$(ASM) -o "bin/${base_boot}.o" -c "$BOOTSECT_SRCS" $(GFLAGS) $(ASFLAGS)

# Create ISO
dd if=/dev/zero of="$ISO" bs=512 count=2880
dd if="bin/$(BOOTSECT)" of="$ISO" conv=notrunc bs=512 seek=0 count=1
dd if="bin/$(KERNEL)" of="$ISO" conv=notrunc bs=512 seek=1 count=2048

echo "Build complete! You can now run it with: qemu-system-i386 -cdrom auroraos.iso"
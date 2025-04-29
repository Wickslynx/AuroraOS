CC=gcc
CXX = g++
NASM=nasm
LD=ld

GFLAGS=
CCFLAGS=-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CCFLAGS+=-Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector
CCFLAGS+=-fno-builtin-function -fno-builtin
CXXFLAGS = -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -nostdlib -Wall -Wextra -O2
NASMFLAGS=-f elf32
LDFLAGS=-m elf_i386

BOOTSECT_SRCS=\
	kernel/boot/boot.S


START_SRCS=\
	kernel/boot/start.S

ASM_SRCS=\


# Convert source file names to object file names
BOOTSECT_OBJS=$(BOOTSECT_SRCS:.S=.o)
START_OBJS=$(START_SRCS:.S=.o)
ASM_OBJS=$(ASM_SRCS:.asm=.o)

# Find all C source files
KERNEL_C_SRCS=$(wildcard kernel/*.c kernel/ux/system/*.c kernel/drivers/*.c kernel/ux/*.c kernel/onstart/*.c kernel/core/*.c)

# Convert C source file names to object file names
KERNEL_C_OBJS=$(KERNEL_C_SRCS:.c=.o)

# All kernel objects combined
KERNEL_OBJS=$(KERNEL_C_OBJS) $(START_OBJS) $(ASM_OBJS)

# Output files
BOOTSECT=bootsect.bin
KERNEL=kernel.bin
ISO=boot.iso

all: dirs bootsect kernel 

dirs:
	mkdir -p bin

clean:
	rm -rf bin
	find kernel -name "*.o" -type f -delete
	rm -f ./*.iso
	rm -f ./**/*.elf
	rm -f ./**/*.bin

# Rule to compile C files
%.o: %.c
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

# Rule to assemble GNU assembler files (.S)
%.o: %.S
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

# Rule to assemble NASM files (.asm)
%.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

# Rule to compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build bootsector
bootsect: $(BOOTSECT_OBJS)
	$(LD) -o ./bin/$(BOOTSECT) $^ -Ttext 0x7C00 --oformat=binary -m elf_i386

# Rule to build kernel
kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/$(KERNEL) $^ $(LDFLAGS) -Tlinker.ld -m elf_i386

# Rule to create bootable ISO
iso: bootsect kernel
	dd if=/dev/zero of=auroraos.iso bs=512 count=2880
	dd if=./bin/$(BOOTSECT) of=auroraos.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/$(KERNEL) of=auroraos.iso conv=notrunc bs=512 seek=1 count=2048

# Special targets
.PHONY: all dirs clean bootsect kernel iso

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
LDFLAGS=-m elf_i386 -g

BOOTSECT_SRCS=\
	kernel/boot/boot.S
BOOTSECT_OBJS=$(BOOTSECT_SRCS:.S=.o)

START_SRCS=\
	kernel/boot/start.S
START_OBJS=$(START_SRCS:.S=.o)

ASM_SRCS=\

# find all C source files
KERNEL_C_SRCS=$(wildcard kernel/*.c kernel/ux/system/*.c kernel/drivers/*.c kernel/ux/*.c kernel/onstart/*.c kernel/core/*.c)
# find all C++ source files
KERNEL_CPP_SRCS=$(wildcard kernel/*.cpp kernel/ux/system/*.cpp kernel/drivers/*.cpp kernel/ux/*.cpp kernel/onstart/*.cpp kernel/core/*.cpp)
# convert C source files to .o
KERNEL_C_OBJS=$(KERNEL_C_SRCS:.c=.o)
# convert C++ source files to .o
KERNEL_CPP_OBJS=$(KERNEL_CPP_SRCS:.cpp=.o)


# all kernel objects combined
KERNEL_OBJS=$(KERNEL_C_OBJS) $(KERNEL_CPP_OBJS) $(START_OBJS) $(ASM_OBJS)

# output files
BOOTSECT=bootsect.bin
KERNEL=kernel.bin
ISO=auroraos.iso

all: dirs bootsect kernel iso

dirs:
	mkdir -p bin

clean:
	rm -rf bin
	find kernel -name "*.o" -type f -delete
	rm -f ./*.iso
	rm -f ./**/*.elf
	rm -f ./**/*.bin

# rule to assemble GNU asm files (.S)
%.o: %.S
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

# rule to assemble NASM files (.asm)
%.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

# rule to compile C files
%.o: %.c
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

# rule to compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# rule to build bootsector
bootsect: $(BOOTSECT_OBJS)
	$(LD) -o ./bin/$(BOOTSECT) $^ -Ttext 0x7C00 --oformat=binary -N -m elf_i386

# rule to build kernel
kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/$(KERNEL) $^ $(LDFLAGS) -Tlinker.ld -m elf_i386

# rule to create bootable ISO (more standard approach using mkisofs)
iso: dirs bootsect kernel
	mkisofs -o $(ISO) -b $(BOOTSECT) -c boot.catalog -no-emul-boot -boot-load-size 4 -boot-info-table bin/

# special targets
.PHONY: all dirs clean bootsect kernel iso

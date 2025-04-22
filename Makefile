CC=gcc
ASM=as
LD=ld

GFLAGS=
CCFLAGS=-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CCFLAGS+=-Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector
CCFLAGS+=-fno-builtin-function -fno-builtin
ASFLAGS=
LDFLAGS= -m

BOOTSECT_SRCS=\
	kernel/boot/boot.S

BOOTSECT_OBJS=$(BOOTSECT_SRCS:.S=.o)

KERNEL_C_SRCS=$(wildcard kernel/*.c)
KERNEL_S_SRCS=$(filter-out $(BOOTSECT_SRCS), $(wildcard kernel/*.S))
KERNEL_OBJS=$(KERNEL_C_SRCS:.c=.o) $(KERNEL_S_SRCS:.S=.o)

BOOTSECT=bootsect.bin
KERNEL=kernel.bin
ISO=boot.iso

all:  bootsect kernel #dirs

#dirs:
#	mkdir -p bin

clean:
	rm ./**/*.o
	rm ./*.iso
	rm ./**/*.elf
	rm ./**/*.bin

%.o: %.c
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

%.o: %.S
	$(ASM) -o $@ -c $< $(GFLAGS) $(ASFLAGS)

bootsect: $(BOOTSECT_OBJS)
	$(LD) -o ./bin/$(BOOTSECT) $^ -Ttext 0x7C00 --oformat=binary

kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/$(KERNEL) $^ $(LDFLAGS) -Tlinker.ld

iso: bootsect kernel
	dd if=/dev/zero of=boot.iso bs=512 count=2880
	dd if=./bin/$(BOOTSECT) of=boot.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/$(KERNEL) of=boot.iso conv=notrunc bs=512 seek=1 count=2048

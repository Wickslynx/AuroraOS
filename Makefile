CC=gcc
ASM=as
LD=ld

GFLAGS=
CCFLAGS=-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CCFLAGS+=-Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector
CCFLAGS+=-fno-builtin-function -fno-builtin
ASFLAGS= --32
LDFLAGS= -m elf_i386

BOOTSECT_SRCS=\
    kernel/boot/stage0.S

BOOTSECT_OBJS=$(BOOTSECT_SRCS:.S=.o)

KERNEL_C_SRCS=$(wildcard kernel/*.c)
KERNEL_S_SRCS=$(filter-out $(BOOTSECT_SRCS), $(wildcard kernel/*.S))
KERNEL_OBJS=$(KERNEL_C_SRCS:.c=.o) $(KERNEL_S_SRCS:.S=.o)

BOOTSECT=bootsect.bin
KERNEL=kernel.bin
FLOPPY_IMG=auroraos.img
ISO=auroraos.iso
BOOT_DIR=boot_iso

all: dirs floppy

# dirs:
#	mkdir -p bin

#clean:
#    rm -rf bin/*.bin *.o *.img $(BOOT_DIR)

%.o: %.c
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

%.o: %.S
	$(ASM) --32 -o $@ -c $< $(GFLAGS) $(ASFLAGS)

bootsect: $(BOOTSECT_OBJS)
	$(LD) -o ./bin/$(BOOTSECT) $^ -Ttext 0x7C00 --oformat=binary

kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/$(KERNEL) $^ $(LDFLAGS) -Tkernel/linker.ld

floppy: bootsect kernel
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880
	dd if=./bin/$(BOOTSECT) of=$(FLOPPY_IMG) conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/$(KERNEL) of=$(FLOPPY_IMG) conv=notrunc bs=512 seek=1 count=$(( $(shell stat -c "%s" ./bin/$(KERNEL)) / 512 + 1 ))
	$(info Build done. Run AuroraoS with: qemu-system-i386 -fda $(FLOPPY_IMG))

iso: bootsect kernel
	mkdir -p $(BOOT_DIR)/boot
	cp ./bin/$(BOOTSECT) $(BOOT_DIR)/boot/bootsect.bin
	echo "boot" > $(BOOT_DIR)/boot.cat
	echo "default boot" >> $(BOOT_DIR)/boot.cat
	echo "boot_info_table" >> $(BOOT_DIR)/boot.cat
	echo "" >> $(BOOT_DIR)/boot.cat
	echo "id \"AuroraOS\"" >> $(BOOT_DIR)/boot.cat
	echo "el torito-alt-boot" >> $(BOOT_DIR)/boot.cat
	echo "  platform=0x00" >> $(BOOT_DIR)/boot.cat
	echo "  bootid=0x01" >> $(BOOT_DIR)/boot.cat
	echo "  load_segment=0x07C0" >> $(BOOT_DIR)/boot.cat
	echo "  system_type=0x00" >> $(BOOT_DIR)/boot.cat
	echo "  boot_status_table=0x01" >> $(BOOT_DIR)/boot.cat
	$(info Creating ISO image: $(ISO))
	genisoimage -o $(ISO) -b boot/bootsect.bin -c boot.cat -no-emul-boot $(BOOT_DIR)
	$(info ISO image created: $(ISO))
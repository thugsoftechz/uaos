# UAOS Makefile
CC = gcc
AS = nasm
LD = ld

# Compiler flags
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

# Directories
SRCDIR = ../src
BUILDDIR = .
INCLUDEDIR = ../include

# Source files
BOOT_ASM = $(SRCDIR)/boot/boot.asm
BOOTLOADER_ASM = $(SRCDIR)/boot/bootloader.asm
KERNEL_SRC = $(SRCDIR)/kernel/kernel.c $(SRCDIR)/kernel/memory.c $(SRCDIR)/kernel/process.c
DRIVER_SRC = $(SRCDIR)/drivers/vga.c $(SRCDIR)/drivers/keyboard.c $(SRCDIR)/drivers/disk.c
GUI_SRC = $(SRCDIR)/gui/gui.c $(SRCDIR)/gui/window.c $(SRCDIR)/gui/events.c
APP_SRC = $(SRCDIR)/apps/file_manager.c $(SRCDIR)/apps/browser.c $(SRCDIR)/apps/media_player.c $(SRCDIR)/apps/package_manager.c
LIB_SRC = $(SRCDIR)/lib/string.c $(SRCDIR)/lib/stdlib.c

# Object files
BOOT_OBJ = boot.o
KERNEL_OBJ = $(KERNEL_SRC:.c=.o)
DRIVER_OBJ = $(DRIVER_SRC:.c=.o)
GUI_OBJ = $(GUI_SRC:.c=.o)
APP_OBJ = $(APP_SRC:.c=.o)
LIB_OBJ = $(LIB_SRC:.c=.o)

ALL_OBJ = $(BOOT_OBJ) $(KERNEL_OBJ) $(DRIVER_OBJ) $(GUI_OBJ) $(APP_OBJ) $(LIB_OBJ)

# Default target
all: uaos.bin bootloader.bin uaos.iso

# Build kernel
uaos.bin: $(ALL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Build bootloader
bootloader.bin: $(BOOTLOADER_ASM)
	$(AS) -f bin -o $@ $^

# Build boot object
$(BOOT_OBJ): $(BOOT_ASM)
	$(AS) $(ASFLAGS) -o $@ $^

# Build C objects
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -o $@ $^

# Create ISO image
uaos.iso: uaos.bin bootloader.bin
	mkdir -p iso/boot/grub
	cp uaos.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso

# Run in QEMU
qemu: uaos.iso
	qemu-system-i386 -cdrom uaos.iso -m 32

# Run with bootloader
qemu-boot: bootloader.bin
	qemu-system-i386 -drive format=raw,file=bootloader.bin -m 32

# Size information
size: uaos.bin
	@echo "UAOS Kernel Size Information:"
	@ls -la uaos.bin
	@size uaos.bin

# Clean build files
clean:
	rm -f *.o *.bin *.iso
	rm -rf iso/

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: all

.PHONY: all clean qemu qemu-boot size debug

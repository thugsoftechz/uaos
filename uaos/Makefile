CC = gcc
ASM = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-builtin -nostdlib -Wall -Wextra -I./include
LDFLAGS = -m elf_i386 -T src/linker.ld

SRC_DIR = src
BUILD_DIR = build

# Find all C files
C_SOURCES = $(shell find $(SRC_DIR) -name "*.c")
# Find all ASM files
ASM_SOURCES = $(shell find $(SRC_DIR) -name "*.asm")

# Create object file names
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

# Final kernel binary
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

all: $(KERNEL_BIN)

# Link everything together
$(KERNEL_BIN): $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

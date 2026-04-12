# ── Compilateurs ─────────────────────────────────
CC      = gcc
ASM     = nasm
LD      = ld

# ── Flags ────────────────────────────────────────
CFLAGS  = -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
ASMFLAGS = -f elf32
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

# ── Dossiers ─────────────────────────────────────
BUILD   = build
SRC_DIRS = kernel

# ── Détection automatique des fichiers ───────────
C_SRCS  = $(shell find $(SRC_DIRS) -name "*.c")
ASM_SRCS = $(shell find $(SRC_DIRS) -name "*.asm")

# ── Génération des objets ─────────────────────────
C_OBJS   = $(patsubst %.c,   $(BUILD)/%.o, $(C_SRCS))
ASM_OBJS = $(patsubst %.asm, $(BUILD)/%.o, $(ASM_SRCS))
OBJS     = $(C_OBJS) $(ASM_OBJS)

# ── Cible principale ──────────────────────────────
all: $(BUILD)/kernel.bin iso

$(BUILD)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# ── Compilation .c ────────────────────────────────
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ── Compilation .asm ──────────────────────────────
$(BUILD)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

iso: boot/kernel.bin
	mkdir -p build
	grub2-mkrescue -o build/OS.iso . --exclude build

# ── Nettoyage ─────────────────────────────────────
clean:
	rm -rf $(BUILD)

# ── Lance dans QEMU ───────────────────────────────
run_bin: $(BUILD)/kernel.bin
	qemu-system-i386 -kernel $(BUILD)/kernel.bin

run: 
	qemu-system-i386 -cdrom build/OS.iso

.PHONY: all clean run
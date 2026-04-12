# ── Compilateurs ─────────────────────────────────
CC       = gcc
ASM      = nasm
LD       = ld

# ── Flags ────────────────────────────────────────
CFLAGS   = -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
           -m32 -Wall -Wextra \
           -I kernel/include

ASMFLAGS = -f elf32
LDFLAGS  = -T linker.ld -nostdlib -m elf_i386

# ── Dossiers ─────────────────────────────────────
BUILD  = build
K      = kernel

# ── Objets explicites ────────────────────────────
OBJS = \
	$(BUILD)/kernel.o            \
	$(BUILD)/inter/isr.o         \
	$(BUILD)/inter/IDT.o         \
	$(BUILD)/vga/vga.o           \
	$(BUILD)/keyboard/keyboard.o \
	$(BUILD)/boot.o              \
	$(BUILD)/inter/isr_asm.o

# ── Cible principale ─────────────────────────────
all: $(BUILD)/OS.iso

# ── Linkage ──────────────────────────────────────
$(BUILD)/kernel.bin: $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

# ── Règles C ─────────────────────────────────────
$(BUILD)/kernel.o: $(K)/src/kernel.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/inter/isr.o: $(K)/src/inter/isr.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/inter/IDT.o: $(K)/src/inter/IDT.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/vga/vga.o: $(K)/src/vga/vga.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard/keyboard.o: $(K)/src/keyboard/keyboard.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ── Règles ASM ───────────────────────────────────
$(BUILD)/boot.o: $(K)/boot.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/inter/isr_asm.o: $(K)/src/inter/isr.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# ── ISO ──────────────────────────────────────────
$(BUILD)/OS.iso: $(BUILD)/kernel.bin boot/grub/grub.cfg
	@mkdir -p $(BUILD)/iso/boot/grub
	cp $(BUILD)/kernel.bin $(BUILD)/iso/boot/kernel.bin
	cp boot/grub/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub2-mkrescue -o $(BUILD)/OS.iso $(BUILD)/iso

# ── Run ──────────────────────────────────────────
run: $(BUILD)/OS.iso
	qemu-system-i386 -cdrom $(BUILD)/OS.iso

run_bin: $(BUILD)/kernel.bin
	qemu-system-i386 -kernel $(BUILD)/kernel.bin

# ── Clean ────────────────────────────────────────
clean:
	rm -rf $(BUILD)

.PHONY: all clean run run_bin
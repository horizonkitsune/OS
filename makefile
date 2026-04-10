CC = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -Wall -Wextra \
         -Ikernel/include

SRCS =  kernel/kernel.c kernel/drivers/vga/vga.c kernel/drivers/keyboard/keyboard.c kernel/inter/IDT.c


OBJS = $(SRCS:.c=.o)

all: boot/kernel.bin iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/kernel.bin: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld -o boot/kernel.bin $(OBJS)

iso: boot/kernel.bin
	mkdir -p build
	grub2-mkrescue -o build/OS.iso . --exclude build

clean:
	rm -f $(OBJS) boot/kernel.bin build/OS.iso

run:
	qemu-system-i386 -cdrom build/OS.iso
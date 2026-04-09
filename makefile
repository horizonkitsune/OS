CC     = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -Wall -Wextra \
         -Isrc/kernel/include

SRCS = src/kernel/kernel.c \
       src/kernel/mm/allocation.c \
       src/kernel/mm/pagination.c \
       src/kernel/process/process.c \
       src/kernel/drivers/vga/vga.c \
       src/kernel/drivers/keyboard/keyboard.c

OBJS = $(SRCS:.c=.o)

all: boot/kernel.bin iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/kernel.bin: $(OBJS)
	mkdir -p boot
	$(CC) $(CFLAGS) -T linker.ld -o boot/kernel.bin $(OBJS)

iso: boot/kernel.bin
	mkdir -p build
	grub-mkrescue -o build/mon_os.iso . --exclude build

clean:
	rm -f $(OBJS) boot/kernel.bin build/mon_os.iso

run:
	qemu-system-i386 -cdrom build/mon_os.iso

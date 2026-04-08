CC = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -Wall -Wextra \
         -I$(shell gcc -m32 -print-file-name=include)

SRCS = kernel/kernel.c kernel/drivers/vga/vga.c
OBJS = $(SRCS:.c=.o)

all: boot/kernel.bin iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/kernel.bin: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld -o boot/kernel.bin $(OBJS)

iso: boot/kernel.bin
	mkdir -p build
	grub2-mkrescue -o build/mon_os.iso .

clean:
	rm -f $(OBJS) boot/kernel.bin build/mon_os.iso

run:
	qemu-system-i386 -cdrom build/mon_os.iso
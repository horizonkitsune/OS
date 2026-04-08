CC = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -Wall -Wextra

SRCS = src/kermel/allocation/allocation.c \
		src/kermel/pagination/pagination.c

OBJS = $(SRCS:.c=.o)

all: boot/kernel.bin iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/kernel.bin: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld -o boot/kernel.bin $(OBJS)

iso:
	grub-mkrescue -o ../mon_os.iso .

clean:
	rm -f $(OBJS) boot/kernel.bin ../mon_os.iso

run:
	qemu-system-i386 -cdrom mon_os.iso
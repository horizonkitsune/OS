CC     = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -Wall -Wextra

SRCS = src/kernel/kernel.c \
       src/kernel/mm/allocation.c \
       src/kernel/mm/pagination.c \
       src/kernel/process/process.c

OBJS = $(SRCS:.c=.o)

all: boot/kernel.bin iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/kernel.bin: $(OBJS)
	$(CC) $(CFLAGS) -T linker.ld -o boot/kernel.bin $(OBJS)

iso:
	grub2-mkrescue -o mon_os.iso .

clean:
	rm -f $(OBJS) boot/kernel.bin mon_os.iso

run:
	qemu-system-i386 -cdrom mon_os.iso

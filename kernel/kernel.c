#define MULTIBOOT_MAGIC  0x1BADB002
#define MULTIBOOT_FLAGS  0x00
#define MULTIBOOT_CHECKSUM -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

__attribute__((section(".multiboot"))) int multiboot_header[] = {
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    MULTIBOOT_CHECKSUM
};

void kernel_main(void) {

    while (1);
}
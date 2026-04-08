section .multiboot
    dd 0x1BADB002       ; magic
    dd 0x00             ; flags
    dd -(0x1BADB002)    ; checksum
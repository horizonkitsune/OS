ENTRY(_start)

SECTIONS {
    . = 0x00100000;

    /* Multiboot header DOIT être en premier */
    .multiboot ALIGN(4) : {
        KEEP(*(.multiboot))
    }

    /* Code */
    .text ALIGN(4) : {
        *(.text*)
    }

    /* Constantes */
    .rodata ALIGN(4) : {
        *(.rodata*)
    }

    /* Données */
    .data ALIGN(4) : {
        *(.data*)
    }

    /* BSS */
    .bss ALIGN(4) : {
        *(COMMON)
        *(.bss*)
    }
}
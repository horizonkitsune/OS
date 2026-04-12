; ── Déclarations globales ISR 0-31 ───────────────
%assign i 0
%rep 32
global isr %+ i
%assign i i+1
%endrep

; ── Déclarations globales IRQ 0-15 ───────────────
%assign i 0
%rep 16
global irq %+ i
%assign i i+1
%endrep

extern isr_handler
extern irq_handler

; ── ISR 0-31 ─────────────────────────────────────
%assign i 0
%rep 32
isr %+ i:
    %if i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17 || i == 21 || i == 29 || i == 30
    push i      ; num_int (code_err déjà poussé par le CPU)
    %else
    push 0      ; code_err factice
    push i      ; num_int
    %endif
    jmp isr_common
%assign i i+1
%endrep

; ── IRQ 0-15 ─────────────────────────────────────
%macro IRQ 1
irq%1:
    push 0
    push %1 + 32
    jmp irq_common
%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

; ── Common ISR ───────────────────────────────────
isr_common:
    pusha
    mov ax, ds
    push eax        ; sauvegarde ds
    mov ax, 0x10    ; segment données kernel
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    call isr_handler
    add esp, 4
    pop eax         ; restaure ds
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    iret

; ── Common IRQ ───────────────────────────────────
irq_common:
    pusha
    mov ax, ds
    push eax        ; sauvegarde ds
    mov ax, 0x10    ; segment données kernel
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    call irq_handler
    add esp, 4
    pop eax         ; restaure ds
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    iret
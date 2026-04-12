; création des isr de 0 à 31
%assign i 0 
%rep 32
global isr %+ i
%assign i i+1
%endrep
; créatioin irq 0 à 15
%assign i 0
%rep 16
global irq %+ i
%assign i i+1
%endrep
; import handlers du isr.c
extern isr_handler
extern irq_handler

%assign i 0
%rep 32
isr %+ i:
    %if i == 8|| i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17 || i == 21 || i == 29 || i == 30 
        push i ; code d'erreur ddéjà poussé
    %else 
        push 0 ; code d'erreur factice
        push i ; num interuption
    %endif             
    jmp isr_common
%assign i i+1
%endrep

%assign i 0
%rep 16
irq %+ i:
    push 0 ; pas de code d'erreur donc 0
    push i + 32
    jmp irq_common
%assign i i+1
%endrep

isr_common:
    pusha ; sauvegardde des registres
    push esp ; passe registers_t* en argument au C
    call isr_handler
    add esp, 4 ; nettoie le push esp
    popa ;restauration des registres
    add esp, 8 ;num int et code d'err nettoyer
    iret ;retour interuption

irq_common:
    pusha
    push esp
    call irq_handler
    add esp, 4
    popa
    add esp, 8
    iret
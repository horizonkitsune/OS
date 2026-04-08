[BITS 16]	;
[ORG 0x7C00]	;l'octet ou se charge le bootloader dans la mémoire

cli ; evite les interuption
xor ax, ax ; mettre ax à 0 
mov ss, ax ; met la valeur de la pile à 0
mov sp, 0x7C00 ; pointeur de pile à l'adresse qui s'agrandit vers le bas
sti ; reactivation des interuption

MOV SI, varTexte ;donne à SI l'octet de debut de varTexte
CALL PrintString	;appelle une fonction

mov si, ReadPacket
mov word[si],      0x10    ; taille du paquet
mov word[si + 2],  0x01    ; 1 secteur à lire
mov word[si + 4],  0x8000  ; destination
mov word[si + 6],  0x00    ; segment
mov dword[si + 8], 0x01    ; bloc LBA 1 = secteur 2
mov dword[si + 12], 0x00   ; partie haute
mov ah, 0x42
mov dl, 0x80               ; disque dur
int 0x13
jc disk_erreur

JMP 0x8000 ; saut à l'addresse du stage 2, chargement stage 2

disk_erreur:
jmp $

PrintString:	;fonction écriture dsur l'écran
MOV AL, [SI]	;sauvegarde l'octet actuelle de SI dans AL
INC SI		;incrementation de SI
OR AL, AL	;vérifie si al = 0 soit la fin de la chaine
JZ exit_function ;si c'est fini alors -> fonction exit
CALL PrintCharacter ;sinon affichage du caractère
JMP PrintString	;saut au prochain caractère de la chaine
exit_function:	;fonction de fin
RET		;Return from procedure

PrintCharacter:	;fonction 
MOV AH, 0x0E	;pour dire au bios d'afficher 1 carctère
MOV BH, 0x00	;Page no., c'est juste ce qu'on voit à l'écran
MOV BL, 0x07	;font gris clair sur fond noir
INT 0x10	;interuption video
RET		;retour au call



;Data
varTexte db 'stage 1, 0x0D, 0x0A, 0	;texte avec valeur 0 comme fin
ReadPacket: times 16 db 0

TIMES 510 - ($ - $$) db 0	;boucle pour remplir le reste des données de 0
DW 0xAA55			;signature bootloader
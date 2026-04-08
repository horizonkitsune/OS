[BITS 16]
[ORG 0x8000]
MOV SI, varTexte
CALL PrintString
JMP $

PrintString:
    MOV AL, [SI]
    INC SI
    OR AL, AL
    JZ exit_function
    CALL PrintCharacter
    JMP PrintString
exit_function:
    RET

PrintCharacter:
    MOV AH, 0x0E
    MOV BH, 0x00
    MOV BL, 0x07
    INT 0x10
    RET

varTexte db 'stage2', 0
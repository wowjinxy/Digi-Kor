.386
.model flat, stdcall
option casemap:none

extrn DAT_004b0200:word

.code
lookupShortFromTable PROC
    mov     eax, [esp + 4]
    and     eax, 0FFFh
    movsx   eax, word ptr [DAT_004b0200 + eax * 2]
    ret
lookupShortFromTable ENDP
END
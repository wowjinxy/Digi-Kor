.386
.model flat, stdcall
option casemap:none

extrn DAT_004b0200:word

.code
FUN_00401020 PROC
    mov     eax, [esp + 4]
    add     eax, 400h
    and     eax, 0FFFh
    movsx   eax, word ptr [DAT_004b0200 + eax * 2]
    ret
FUN_00401020 ENDP
END
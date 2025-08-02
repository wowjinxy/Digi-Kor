.386
.model flat, stdcall
option casemap:none

extrn FUN_00401020:PROC

.code
FUN_00401040 PROC
    mov     eax, [esp + 4]
    push    eax
    call    FUN_00401020
    add     esp, 4
    ret
FUN_00401040 ENDP
END
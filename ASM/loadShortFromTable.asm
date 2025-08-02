.386
.model flat, stdcall
option casemap:none

extrn lookupShortFromTable:PROC

.code
loadShortFromTable PROC
    mov     eax, [esp + 4]
    push    eax
    call    lookupShortFromTable
    add     esp, 4
    ret
loadShortFromTable ENDP
END
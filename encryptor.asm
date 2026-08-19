.code
XorCipher PROC EXPORT
    ; RCX = file buffer address
    ; RDX = file size in bytes
    ; R8B = key

    ; if file size = 0 -> exit
    cmp rdx, 0
    jle Code_Exit

Loop_Start:
    mov al, byte ptr [rcx]   ; get file byte
    xor al, r8b              ; xor with key
    mov byte ptr [rcx], al   ; rewrite

    inc rcx                  ; next byte
    dec rdx                  ; counter
    jnz Loop_Start           

Code_Exit:
    ret
XorCipher ENDP
END

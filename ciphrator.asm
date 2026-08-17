.code
XorCipher PROC EXPORT
    ; RCX = адрес буфера с файлом
    ; RDX = размер файла в байтах
    ; R8B = ключ (третий аргумент в x64 летит в R8, младший байт R8B)

    ; Проверка безопасности: если размер файла <= 0, сразу выходим
    cmp rdx, 0
    jle Code_Exit

Loop_Start:
    mov al, byte ptr [rcx]   ; Берем байт из файла
    xor al, r8b              ; Ксорим с ключом (теперь он в R8B, т.к. стал 3-м аргументом)
    mov byte ptr [rcx], al   ; Записываем обратно

    inc rcx                  ; Двигаемся к следующему байту
    dec rdx                  ; Уменьшаем счетчик оставшихся байт
    jnz Loop_Start           ; Если байты еще остались — крутим цикл

Code_Exit:
    ret
XorCipher ENDP
END

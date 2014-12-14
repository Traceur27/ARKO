    section .text
    global pokoloruj

pokoloruj:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp + 8] ;  adres na tablice w edx
    xor eax, eax ; eax - najwieksza znaleziona liczba

	mov al, 0x00
    mov [edx+122], al
    mov [edx+123], al
    mov [edx+124], al
   

    pop ebp
    ret


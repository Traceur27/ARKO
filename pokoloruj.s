;zadanie polegalo na zaimplemtowaniu funkcji ktora dla kazdego
;znalezionego w ciagu znakow łańcucha składającego sie z liczb
;po skonwertowanu go na int porownuje z pozostałymi i zwraca
;najwiekszy w postaci unsigned int
;
;unsigned int bignum(const char * string);
;
;przyklad uzycia
;unsigned int a = bignum("100asd200 300 asd100");
;printf("%u\n",a);
;rezultat:
;300

    section .text
    global pokoloruj

pokoloruj:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp + 8] ;  adres na tablice w edx
    ;xor ecx, ecx ; sprawdzany znak
    ;xor ebx, ebx ; w ebx bedziemy trzymac liczbe kowertowana i bedziemy w nim akumulowac cyfry
    xor eax, eax ; eax - najwieksza znaleziona liczba

	mov al, 0x00
    mov [edx+122], al
    mov [edx+123], al
    mov [edx+124], al
   

    pop ebp
    ret


global blend

    section .data
    x: dd 0.5                      ;declare double word(4 bajty)

section .text

blend:
    push rbp
    mov rbp, rsp
	sub rsp, 8
	
	mov r10, [rbp + 16]        ;tablica do pisania w r10

	xor rbx, rbx
	xor rax, rax
	mov eax, dword [rcx + 12]  ;biore wysokosc malego
	mov ebx, dword [rcx + 8]   ;oraz szerokosc

	imul ebx, eax              ;liczba pixeli w malej tablicy[rbx] - bedzie potrzebna przy petli
	
	
	mov eax, [rdx +20]         ;pobieram szerokosc duzego w bajtach
	dec esi                    ;punkt nalozenia sie - wysokosc

	imul eax, esi              ;liczba bajtow w szerokosci * poczatkowy y
	dec edi                    ;punkt nalozenia sie - szerokosc
	mov r11, rdi
	imul r11d, 3
	
	add eax, r11d              ;liczba bajtow do punktu startowego juz jest w rax - all good
	
	add r10, rax		       ;przesuniecie sie o odpowiednia liczbe bajtow
	add r8, rax                ;przesuniecie na odpowiedni pixel w tablicy z duzymi pixelami 


	fld dword [x]              ;wspolczynnik przezroczystosci
	fld1                       ;1 w st0, x w st1
	fsub st0, st1              ;1-x w st0, x w st1
	xor r12, r12
	xor r13, r13


	loop:     
	cmp r12d, ebx     	   ;czy juz caly maly obrazek
	je end

    cmp r13d, [rcx + 8]            ;czy linijka sie skonczyla
	je next


	;B
	xor r14, r14		   ;wyzerowanie rejestrow
	xor r15, r15

	mov r14b, [r8]             ;pobranie wartosci jednego koloru
	mov r15b, [r9]             ;oraz drugiego - hope it;s good to this moment
	
	mov [rbp - 4], dword r14d
	mov [rbp - 8], dword r15d


	fild dword [rbp - 4]       ;kolor jednego w st0, st1 1-x i x w st2
	fild dword [rbp - 8]       ;kolor 2 w st0, kolor 1 w st1, 1-x w st2 i x w st3 
	fmul st0, st2              ;kolor2 * 1-x
	fistp dword [rbp - 4]      ;do pamieci; kolor1 w st0, 1-x w st1 i x w st2
	mov r14d, dword [rbp - 4]


	fmul st0, st2              ;kolor1 * x
	fistp dword [rbp - 8]      ;drugie slowo do pamieci
	add r14d, dword[rbp - 8]
	mov [r10], r14b            ;wrzucam do tablicy wynikowej
	inc r10


	;G	
	xor r14, r14		   ;wyzerowanie rejestrow
	xor r15, r15

	mov r14b, [r8+1]           ;pobranie wartosci jednego koloru
	mov r15b, [r9+1]           ;oraz drugiego - hope it;s good to this moment
	
	mov [rbp - 4], dword r14d
	mov [rbp - 8], dword r15d


	fild dword [rbp - 4]       ;kolor jednego w st0, st1 1-x i x w st2
	fild dword [rbp - 8]       ;kolor 2 w st0, kolor 1 w st1, 1-x w st2 i x w st3 
	fmul st0, st2              ;kolor2 * 1-x
	fistp dword [rbp - 4]      ;do pamieci; kolor1 w st0, 1-x w st1 i x w st2
	mov r14d, dword [rbp - 4]


	fmul st0, st2              ;kolor1 * x
	fistp dword [rbp - 8]      ;drugie slowo do pamieci
	add r14d, dword[rbp - 8]
	mov [r10], r14b            ;wrzucam do tablicy wynikowej
	inc r10


	;R
	xor r14, r14		   ;wyzerowanie rejestrow
	xor r15, r15

	mov r14b, [r8+2]           ;pobranie wartosci jednego koloru
	mov r15b, [r9+2]           ;oraz drugiego - hope it;s good to this moment
	
	mov [rbp - 4], dword r14d
	mov [rbp - 8], dword r15d


	fild dword [rbp - 4]       ;kolor jednego w st0, st1 1-x i x w st2
	fild dword [rbp - 8]       ;kolor 2 w st0, kolor 1 w st1, 1-x w st2 i x w st3 
	fmul st0, st2              ;kolor2 * 1-x
	fistp dword [rbp - 4]      ;do pamieci; kolor1 w st0, 1-x w st1 i x w st2
	mov r14d, dword [rbp - 4]


	fmul st0, st2              ;kolor1 * x
	fistp dword [rbp - 8]      ;drugie slowo do pamieci
	add r14d, dword[rbp - 8]
	mov [r10], r14b            ;wrzucam do tablicy wynikowej
	inc r10
	

	add r8, 3
	add r9, 3
	inc r12                    ;inkrementacja licznikow
	inc r13
	jmp loop
	
   	
	next:
	imul r13d, 3               ;liczba pixeli w wierszu, ktore zostaly spisane zamieniana na bajty
	sub r8, r13                ;cofniecie edx na poczatek wiersza
	xor r15, r15
	mov r15d, dword[rdx + 20]
	add r8, r15                ;i przeskoczenie jeden wiersz do gory
	sub r10, r13
	add r10, r15               ;to samo z r10
	xor r11, r11
	mov r11d, dword[rcx + 20]  ;trzeba jeszcze uwzglednic padding - pobieram szerokosc malego w bajtach
	sub r11d, r13d             ;odejmuje od policzonej szerokosci w bajtach
	add r9, r11                ;i przesuwam wskaznik z malego obrazka
	xor r13, r13               ;licznik pixeli w wierszu od poczatku

	jmp loop

	end:
		
	mov rsp, rbp               ;dealokacja zmiennych lokalnych
    pop rbp
    ret

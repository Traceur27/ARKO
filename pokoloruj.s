    section .text
    global pokoloruj

pokoloruj:
        push ebp
        mov ebp, esp
	sub esp, 20
	
	push edi
	push esi
	push ebx


	mov eax, [ebp + 20]        ;odwolanie sie do stosu, na ktorym jest struktura z danymi malego obrazka
	add eax, 12
	mov ebx, [eax]             ;biore wysokosc
	sub eax, 4
	mov ecx, [eax]             ;oraz szerokosc
	imul ebx, ecx
	mov [ebp - 4], ebx         ;liczba pixeli w malej tablicy - bedzie potrzebna przy petli
	mov ebx, [eax]
	mov [ebp - 8], ebx         ;szerokosc malego zapamietana
	

	mov eax, [ebp + 16]
	add eax, 20
	mov ebx, [eax]
	mov [ebp - 12], ebx        ;liczba bajtow szerokosci duzego obrazka

	mov dword [ebp - 16], 0
	mov dword [ebp - 20], 0    ;zajecie dwoch miejsc dla licznikow
	
	
	mov edx, [ebp + 16]        ;pobieram szerokosc duzego w bajtach
	add edx, 20
	mov eax, [edx]
	mov edx, [ebp + 12]        ;punkt nalozenia sie - wysokosc
	dec edx
	imul eax, edx              ;liczba bajtow w szerokosci
	mov edx, [ebp + 8]         ;punkt nalozenia sie - szerokosc
	dec edx
	imul edx, 3
	add eax, edx
	mov edx, eax               ;liczba bajtow do punktu startowego juz jest w edx
    
	
        mov esi, [ebp + 32]	   ;adres na tablice po ktorej mam pisac
	add esi, edx		   ;przesuniecie sie o odpowiednia liczbe bajtow
	mov eax, [ebp + 24]        ;adres pierwszej tablicy
	add eax, edx               ;przesuniecie na odpowiedni pixel
	mov edx, eax	           ;zapamietanie w edx
	mov eax, [ebp + 28]        ;adres na tablice z malym obrazkiem
	
	

	loop:
	mov edi, [ebp - 20]     
	cmp edi, [ebp - 4]	   ;czy juz caly maly obrazek
	je end

	mov edi, [ebp - 16]
        cmp [ebp - 8], edi         ;czy linijka sie skonczyla
	je next

	mov bl, [edx]              ;pobranie wartosci jednego koloru
	mov cl, [eax]              ;oraz drugiego
	shr bl, 1                  ;odpowiednia czesc pierwszego
	shr cl, 1
	add bl, cl                 ;suma tych czesci
	mov [esi], bl              ;wrzucam do tablicy wynikowej
	mov bl, [edx+1]            ;i powtarzam jeszcze dwa razy, poniewaz przesuwam sie po pixelach(jeden to trzy bajty)
	mov cl, [eax+1]
	shr bl, 1
	shr cl, 1
	add bl, cl  
	mov [esi + 1], bl
	mov bl, [edx+2]
	mov cl, [eax+2]
	shr bl, 1
	shr cl, 1
	add bl, cl
	mov [esi + 2], bl

	
	add esi, 3                 ;przesuniecie wskaznikow na nastepne pixele
	add edx, 3
	add eax, 3
	inc dword [ebp - 20]       ;inkrementacja licznikow
	inc dword [ebp - 16]
	jmp loop
	
   	
	next:
	mov edi, [ebp - 16]
	imul edi, 3                ;liczba pixeli w wierszu, ktore zostaly spisane zamieniana na bajty
	push edi
	sub edx, edi               ;cofniecie edx na poczatek wiersza
	add edx, [ebp - 12]        ;i przeskoczenie jeden wiersz do gory
	sub esi, edi
	add esi, [ebp - 12]        ;to samo z esi
	mov dword [ebp - 16], 0    ;licznik pixeli w wierszu od poczatku
	mov edi, [ebp + 20]        ;trzeba jeszcze uwzglednic padding
	mov edi, [edi + 20]        ;pobieram szerokosc malego w bajtach
	sub edi, [ebp - 36]        ;odejmuje od policzonej szerokosci w bajtach
	add eax, edi               ;i przesuwam wskaznik z malego obrazka
	pop edi                    ;to juz nie potrzebne
	jmp loop

	end:
	
	pop ebx                    ;przywrocenie zapamietanych rejestrow
	pop esi
	pop edi
		
	mov esp, ebp               ;dealokacja zmiennych lokalnych

        pop ebp
        ret


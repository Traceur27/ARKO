    section .text
    global pokoloruj

pokoloruj:
    push ebp
    mov ebp, esp
	;sub esp, 20            ;przygotowanie miejsca na stosie

	mov eax, [ebp + 12]
	add eax, 12
	mov ebx, [eax]         ;biore wysokosc
	sub eax, 4
	mov ecx, [eax]         ;oraz szerokosc
	imul ebx, ecx
	push ebx               ;liczba pixeli w malej tablicy - bedzie potrzebna przy petli
	push dword [eax]        ;szerokosc malego
	

	mov eax, [ebp + 8]
	add eax, 20
	push dword [eax]        ;liczba bajtow szerokosci duzego obrazka

	mov dword [ebp - 16], 1
	mov dword [ebp - 20], 1 ;zajecie dwoch miejsc dla licznikow
	
	
	mov edx, [ebp + 8]
	add edx, 20
	mov eax, [edx]
	mov edx, 10           ;punkt nalozenia sie - wysokosc
	dec edx
	imul eax, edx          ;liczba bajtow w szerokosci
	mov edx, 10            ;punkt nalozenia sie - szerokosc
	dec edx
	imul edx, 3
	add eax, edx
	mov edx, eax           ;liczba bajtow do punktu startowego juz jest w edx
    
	
    mov esi, [ebp + 24]		;adres na tablice po ktorej mam pisac
	add esi, edx			;przesuniecie sie o odpowiednia liczbe bajtow
	mov eax, [ebp + 16]    ;adres pierwszej tablicy
	add eax, edx           ;przesuniecie na odpowiedni pixel
	mov edx, eax	       ;zapamietanie w edx
	mov eax, [ebp + 20]     ;adres na tablice z malym obrazkiem
	
	

	loop:
	mov edi, [ebp - 20]     
	cmp edi, [ebp - 4]	    ;czy juz caly maly obrazek
	je end

	mov edi, [ebp - 16]
    cmp [ebp - 8], edi          ;czy linijka sie skonczyla
	je next

	mov bl, [edx]
	mov cl, [eax]
	shr bl, 1
	shr cl, 1
	add bl, cl
	mov [esi], bl    ;tu sie psuje
	mov [esi + 1], bl
	mov [esi + 2], bl

	
	add esi, 3
	add edx, 3
	add eax, 3
	inc dword [ebp - 16]
	inc dword [ebp - 20]
	jmp loop
	
   	
	next:
	mov edi, [ebp - 16]
	imul edi, 3            ;liczba pixeli w wierszu, ktore zostaly spisane zamieniana na bajty
	sub edi, 3             ;odejmuje 3, bo po petli wyladawalem za daleko
	sub edx, edi           ;cofniecie edx na poczatek wiersza
	add edx, [ebp - 12]    ;i przeskoczenie jeden wiersz do gory
	sub esi, edi
	add esi, [ebp - 12]    ;to samo z esi
	mov dword [ebp - 16], 1 ;licznik pixeli w wierszu od poczatku
	jmp loop

	end:
	mov esp, ebp           ;dealokacja zmiennych lokalnych
    pop ebp
    ret


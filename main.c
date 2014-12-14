#include <stdio.h>
#include <stdlib.h>

void pokoloruj(char * p);

int main()
{
    const char * nazwa = "abc.bmp";
    FILE * plik;
    int a;
    //char b;

    if((plik = fopen(nazwa, "rb")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    fseek(plik, 2L, SEEK_SET);
    fread(&a, sizeof(int),1,plik);
    rewind(plik);
    char tab[a];
    fread(tab, sizeof(char), a, plik);
    //tab[54] = 0x00;
    //tab[55] = 0x00;
    //tab[56] = 0x00;
    pokoloruj(tab);
    fclose(plik);
     if((plik = fopen("cba.bmp", "wb")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    fwrite(tab, sizeof(char), a, plik);
    fclose(plik);
    //printf("Rozmiar pliku %d", a);
	//sa

return 0;
}



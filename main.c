#include <stdio.h>
#include <stdlib.h>

struct Naglowek
{
    int rozmiar;
    int offset;
    int szerokosc;
    int wysokosc;
    int rozmiar_tablicy;
    int szerokosc_bajty;
};

void pokoloruj(struct Naglowek *, struct Naglowek *, char *, char *, char *);

int main()
{
    struct Naglowek one, two;
    const char * nazwa1 = "abc.bmp";
    const char * nazwa2 = "cba.bmp";
    FILE * plik, *bufor;
    int a;
    //char b;

    //usupelnienie naglowka
    if((plik = fopen(nazwa1, "rb")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    fseek(plik, 2L, SEEK_SET);
    fread(&(one.rozmiar), sizeof(int),1,plik);
    fseek(plik, 4L, SEEK_CUR);
    fread(&(one.offset), sizeof(int),1,plik);
    fseek(plik, 4L, SEEK_CUR);
    fread(&(one.szerokosc), sizeof(int),1,plik);
    fread(&(one.wysokosc), sizeof(int),1,plik);
    fseek(plik, 8L, SEEK_CUR);
    fread(&(one.rozmiar_tablicy), sizeof(int),1,plik);
    one.szerokosc_bajty = one.rozmiar_tablicy/one.wysokosc;
    char tab1[one.rozmiar_tablicy];
    fseek(plik, (long) one.offset, SEEK_SET);
    fread(tab1, sizeof(char),one.rozmiar_tablicy,plik);


    if((bufor = fopen("bufor.bmp", "wb")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    rewind(plik);
    char tab[one.rozmiar];
    fread(tab, sizeof(char), one.rozmiar, plik);
    fwrite(tab, sizeof(char), one.rozmiar, bufor);

     fclose(plik);
     fclose(bufor);


    if((plik = fopen(nazwa2, "rb")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    fseek(plik, 2L, SEEK_SET);
    fread(&(two.rozmiar), sizeof(int),1,plik);
    fseek(plik, 4L, SEEK_CUR);
    fread(&(two.offset), sizeof(int),1,plik);
    fseek(plik, 4L, SEEK_CUR);
    fread(&(two.szerokosc), sizeof(int),1,plik);
    fread(&(two.wysokosc), sizeof(int),1,plik);
    fseek(plik, 8L, SEEK_CUR);
    fread(&(two.rozmiar_tablicy), sizeof(int),1,plik);
    two.szerokosc_bajty = two.rozmiar_tablicy/two.wysokosc;
    char tab2[two.rozmiar_tablicy];
    fseek(plik, (long) two.offset, SEEK_SET);
    fread(tab1, sizeof(char),two.rozmiar_tablicy,plik);
    fclose(plik);

 /*    if((plik = fopen("test.bmp", "a+b")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    fseek(plik, (long) one.offset,SEEK_SET );
    fwrite(tab1, sizeof(char), one.rozmiar_tablicy, plik);
    fclose(plik); */

	pokoloruj(&one, &two, tab1,tab2, tab);

	if((bufor = fopen("bufor.bmp", "rb+")) == NULL)
    	{
    	    fprintf(stderr, "Blad otwarcia pliku\n");
    	    exit(1);
    	}
	 fseek(bufor, (long) one.offset, SEEK_SET);
	 //fwrite(tab, sizeof(char), one.rozmiar_tablicy, bufor);
	 fclose(bufor);
	
    printf("Rozmiar pliku %d, offset %d, szerokosc %d, wysokosc %d, rozmiar tablicy %d, bajty %d\n\n", one.rozmiar, one.offset, one.szerokosc
           ,one.wysokosc, one.rozmiar_tablicy, one.szerokosc_bajty);
    printf("Rozmiar pliku 2 %d, offset %d, szerokosc %d, wysokosc %d, rozmiar tablicy %d, bajty %d", two.rozmiar, two.offset, two.szerokosc
           ,two.wysokosc, two.rozmiar_tablicy, two.szerokosc_bajty);

return 0;
}


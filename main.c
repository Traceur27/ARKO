#include <stdio.h>
#include <stdlib.h>
#define wysokoscStartowa 25
#define szerokoscStartowa 25

struct Naglowek
{
    int rozmiar;
    int offset;
    int szerokosc;
    int wysokosc;
    int rozmiar_tablicy;
    int szerokosc_bajty;
};

int pokoloruj(int xStartowe, int yStartowe, struct Naglowek*, struct Naglowek*, char *pixeleDuzego, char *pixeleMalego, char *tablicaDoPisania);

int main()
{
    struct Naglowek one, two;
    const char * nazwa1 = "duzy.bmp";
    const char * nazwa2 = "maly.bmp";
    FILE * plik, *bufor;

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

    char duzySamePixele[one.rozmiar_tablicy]; //tablica w ktorej bede przechowywal same pixele z duzego obrazka
    fseek(plik, (long) one.offset, SEEK_SET); //przsun sie o offset
    fread(duzySamePixele, sizeof(char),one.rozmiar_tablicy,plik); //przepisz wszystko do konca


    if((bufor = fopen("wynik.bmp", "wb")) == NULL)
    {
        fprintf(stderr, "Blad otwarcia pliku\n");
        exit(1);
    }
    rewind(plik);


    char duzyKopia[one.rozmiar]; //tablica na kopie duzego obrazka wejsciowego
    fread(duzyKopia, sizeof(char), one.rozmiar, plik);  //spisuje wartosci
    fwrite(duzyKopia, sizeof(char), one.rozmiar, bufor);  //i od razu tworze plik bmp bedacy moim buforem
	rewind(plik);

	char tablicaDoPisania[one.rozmiar_tablicy]; //tablica dla samych pixeli 
	fseek(plik, (long)one.offset, SEEK_SET);
	fread(tablicaDoPisania, sizeof(char), one.rozmiar_tablicy, plik); 	
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


    char malySamePixele[two.rozmiar_tablicy]; //tablica na pixele z malego obrazka
    fseek(plik, (long) two.offset, SEEK_SET);
    fread(malySamePixele, sizeof(char),two.rozmiar_tablicy,plik);
    fclose(plik);


	int z = pokoloruj(szerokoscStartowa, wysokoscStartowa,&one, &two, duzySamePixele,malySamePixele, tablicaDoPisania);
	printf("Zmienna zwrocona: %d\n", z);

	if((bufor = fopen("wynik.bmp", "rb+")) == NULL)
    	{
    	    fprintf(stderr, "Blad otwarcia pliku\n");
    	    exit(1);
    	}
	 
	 fseek(bufor, (long) one.offset, SEEK_SET);
	 fwrite(tablicaDoPisania, sizeof(char), one.rozmiar_tablicy, bufor);
	 fclose(bufor);
	
    printf("Rozmiar pliku %d, offset %d, szerokosc %d, wysokosc %d, rozmiar tablicy %d, bajty %d\n\n", one.rozmiar, one.offset, one.szerokosc
           ,one.wysokosc, one.rozmiar_tablicy, one.szerokosc_bajty);
    printf("Rozmiar pliku 2 %d, offset %d, szerokosc %d, wysokosc %d, rozmiar tablicy %d, bajty %d", two.rozmiar, two.offset, two.szerokosc
           ,two.wysokosc, two.rozmiar_tablicy, two.szerokosc_bajty);

return 0;
}


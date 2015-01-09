#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#define wysokoscStartowa 25
#define szerokoscStartowa 25
const float FPS = 60;

enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

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

	if( ((szerokoscStartowa + two.szerokosc) > one.szerokosc) || ((wysokoscStartowa + two.wysokosc) > one.wysokosc) )
	{
		printf("Blad. Maly obrazek wychodzi poza obszar duzego\n");
		exit(1);
	}	

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
    printf("Rozmiar pliku 2 %d, offset %d, szerokosc %d, wysokosc %d, rozmiar tablicy %d, bajty %d\n\n", two.rozmiar, two.offset, two.szerokosc
           ,two.wysokosc, two.rozmiar_tablicy, two.szerokosc_bajty);

	
	int wysokosc = wysokoscStartowa;
	int szerokosc  = szerokoscStartowa;




	ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool key[4] = { false, false, false, false };
    bool redraw = true;
    bool doexit = false;

    if(!al_init())
    {
       fprintf(stderr, "failed to initialize allegro!\n");
       return -1;
     }
 
    if(!al_install_keyboard()) 
    {
       fprintf(stderr, "failed to initialize the keyboard!\n");
       return -1;
     }
 
     timer = al_create_timer(1.0 / FPS);
    if(!timer) 
    {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }
 
    display = al_create_display(one.szerokosc, one.wysokosc);
    if(!display) 
    {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
    }
 

	 event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
	
   al_register_event_source(event_queue, al_get_display_event_source(display));
 
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
 
   al_register_event_source(event_queue, al_get_keyboard_event_source());

	int licznikPoziomy = 0;
	int licznikPionowy  = 0;
	int kolor = 0;
	int wyrownianie = (one.szerokosc_bajty - 3*one.szerokosc);
	
	for(;licznikPionowy < one.wysokosc; ++licznikPionowy)
	{
		for(;licznikPoziomy < one.szerokosc; ++licznikPoziomy)
		{
		al_put_pixel(licznikPoziomy,one.wysokosc - licznikPionowy,al_map_rgb(tablicaDoPisania[kolor+2],tablicaDoPisania[kolor+1],tablicaDoPisania[kolor]));
		kolor +=3;
		}
	licznikPoziomy = 0;
	kolor += wyrownianie;
	} 
	
	al_flip_display();


	al_start_timer(timer);


	   while(!doexit)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         if(key[KEY_UP] && (wysokosc + 10 <= one.wysokosc)) {
            wysokosc += 10;
         }
 
         if(key[KEY_DOWN] && (wysokosc -10 >= 0)) {
            wysokosc -= 10;
         }
 
         if(key[KEY_LEFT] && (szerokosc - 10 >= 0)) {
            szerokosc -= 10;
         }
 
         if(key[KEY_RIGHT] && (szerokosc +10 <= one.szerokosc)) {
            szerokosc += 10;
         }
 
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
			   redraw = true;
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
			   redraw = true;
               break;
 
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = true;
			   redraw = true;
               break;
 
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;
			   redraw = true;
               break;
         }
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;
 
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = false;
               break;
 
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = false;
               break;
 
            case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }
      }
 
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
	pokoloruj(szerokosc, wysokosc,&one, &two, duzySamePixele,malySamePixele, tablicaDoPisania);
 
         for(kolor=0,licznikPionowy=0,licznikPoziomy=0;licznikPionowy < one.wysokosc; ++licznikPionowy)
			{
				for(;licznikPoziomy < one.szerokosc; ++licznikPoziomy)
				{
				al_put_pixel(licznikPoziomy,one.wysokosc - licznikPionowy,al_map_rgb(tablicaDoPisania[kolor+2],tablicaDoPisania[kolor+1],tablicaDoPisania[kolor]));
				kolor +=3;
				}
			licznikPoziomy = 0;
			kolor += wyrownianie;
			}
         al_flip_display();
      }
   }


		if((bufor = fopen("wynik.bmp", "rb+")) == NULL)
		{
		fprintf(stderr, "Blad otwarcia pliku\n");
		exit(1);
		}

		 fseek(bufor, (long) one.offset, SEEK_SET);
		 fwrite(tablicaDoPisania, sizeof(char), one.rozmiar_tablicy, bufor);
		 fclose(bufor);


   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);

return 0;
}


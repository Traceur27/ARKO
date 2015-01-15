#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
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

void pokoloruj(int xStartowe, int yStartowe, struct Naglowek*, struct Naglowek*, char *pixeleDuzego, char *pixeleMalego, char *tablicaDoPisania);

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

	pokoloruj(szerokoscStartowa, wysokoscStartowa,&one, &two, duzySamePixele,malySamePixele, tablicaDoPisania);

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
	ALLEGRO_BITMAP  *image   = NULL;
    bool redraw = true;
    bool doexit = false;

    if(!al_init())
    {
       fprintf(stderr, "failed to initialize allegro!\n");
       return -1;
     }
 
	if(!al_init_image_addon()) 
    {
       fprintf(stderr, "failed to initialize image!\n");
       return 0;
    }

    if(!al_install_keyboard()) 
    {
       fprintf(stderr, "failed to initialize the keyboard!\n");
       return -1;
     } 
 
    display = al_create_display(one.szerokosc, one.wysokosc);
    if(!display) 
    {
      fprintf(stderr, "failed to create display!\n");
      return -1;
    }
 

	 event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      return -1;
   }
	
   al_register_event_source(event_queue, al_get_display_event_source(display));
 
   al_register_event_source(event_queue, al_get_keyboard_event_source());

	image = al_load_bitmap("wynik.bmp");
	al_draw_bitmap(image, 0, 0, 0);
	al_flip_display();


   while(!doexit)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 

	if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
				if(wysokosc + two.wysokosc +5 <= one.wysokosc)
				{
			     redraw = true;
				 wysokosc += 5;
				}
               break;
 
            case ALLEGRO_KEY_DOWN:
				if(wysokosc -5 >= 0)
				{
			   	redraw = true;
				wysokosc -=5;
				}
               break;
 
            case ALLEGRO_KEY_LEFT: 
				if(szerokosc -5 >= 0)
				{
			   	redraw = true;
				szerokosc -=5;
				}
               break;
 
            case ALLEGRO_KEY_RIGHT:
				if(szerokosc +two.szerokosc + 5 <= one.szerokosc)
				{
			   	redraw = true;
				szerokosc +=5;
				}
               break;

			case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }
      }
 
      if(redraw){
         redraw = false;
			int i = 0;
			for(;i<one.rozmiar_tablicy;++i)
			tablicaDoPisania[i] = duzySamePixele[i];
	pokoloruj(szerokosc, wysokosc,&one, &two, duzySamePixele,malySamePixele, tablicaDoPisania);
 
	if((bufor = fopen("wynik.bmp", "rb+")) == NULL)
		{
		fprintf(stderr, "Blad otwarcia pliku\n");
		exit(1);
		}

		 fseek(bufor, (long) one.offset, SEEK_SET);
		 fwrite(tablicaDoPisania, sizeof(char), one.rozmiar_tablicy, bufor);
		 fclose(bufor);
		
		image = al_load_bitmap("wynik.bmp");
		al_set_target_bitmap(al_get_backbuffer(display));
		al_draw_bitmap(image, 0, 0, 0);
         al_flip_display();
      }
   }


   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   al_destroy_bitmap(image);

return 0;
}


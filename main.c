#include <stdlib.h>
#include <stdio.h>

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"

#define START_HEIGHT 25
#define START_WIDTH 25
#define RESULT_FILENAME "result.bmp"


struct Header
{
    int size;
    int offset;
    int width;
    int height;
    int tableSize;
    int widthInBytes;
};

void blend(int startingX, int startingY, struct Header *, struct Header *, char *pixelsFromBig, char *pixelsFromSmall,
           char *tableToWrite);

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: ./blend <path_to_bigger_img> <path_to_smaller_img>\n");
        exit(1);
    }

    struct Header one, two;
    const char * filename1 = argv[1];
    const char * filename2 = argv[2];
    FILE * file, *buffer;

    //Fill in the header
    if((file = fopen(filename1, "rb")) == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }
    fseek(file, 2L, SEEK_SET);
    fread(&(one.size), sizeof(int),1,file);
    fseek(file, 4L, SEEK_CUR);
    fread(&(one.offset), sizeof(int),1,file);
    fseek(file, 4L, SEEK_CUR);
    fread(&(one.width), sizeof(int),1,file);
    fread(&(one.height), sizeof(int),1,file);
    fseek(file, 8L, SEEK_CUR);
    fread(&(one.tableSize), sizeof(int),1,file);
    one.widthInBytes = one.tableSize/one.height;

    char onlyPixelsFromBig[one.tableSize];
    fseek(file, (long) one.offset, SEEK_SET);
    fread(onlyPixelsFromBig, sizeof(char),one.tableSize,file);


    if((buffer = fopen(RESULT_FILENAME, "wb")) == NULL)
    {
        fprintf(stderr, "Error opening result file\n");
        exit(1);
    }
    rewind(file);


    char copyOfBig[one.size];
    fread(copyOfBig, sizeof(char), one.size, file);
    fwrite(copyOfBig, sizeof(char), one.size, buffer);
    rewind(file);

    char tableToWrite[one.tableSize];
    fseek(file, (long)one.offset, SEEK_SET);
    fread(tableToWrite, sizeof(char), one.tableSize, file);
    fclose(file);
    fclose(buffer);


    if((file = fopen(filename2, "rb")) == NULL)
    {
        fprintf(stderr, "Error opening second file\n");
        exit(1);
    }
    fseek(file, 2L, SEEK_SET);
    fread(&(two.size), sizeof(int),1,file);
    fseek(file, 4L, SEEK_CUR);
    fread(&(two.offset), sizeof(int),1,file);
    fseek(file, 4L, SEEK_CUR);
    fread(&(two.width), sizeof(int),1,file);
    fread(&(two.height), sizeof(int),1,file);
    fseek(file, 8L, SEEK_CUR);
    fread(&(two.tableSize), sizeof(int),1,file);
    two.widthInBytes = two.tableSize/two.height;


    char onlyPixelsFromSmall[two.tableSize];
    fseek(file, (long) two.offset, SEEK_SET);
    fread(onlyPixelsFromSmall, sizeof(char),two.tableSize,file);
    fclose(file);

    if( ((START_WIDTH + two.width) > one.width) || ((START_HEIGHT + two.height) > one.height) )
    {
        printf("Error. Smaller picture is bigger then big one.\n");
        exit(1);
    }

    blend(START_WIDTH, START_HEIGHT, &one, &two, onlyPixelsFromBig, onlyPixelsFromSmall, tableToWrite);

    if((buffer = fopen(RESULT_FILENAME, "rb+")) == NULL)
    {
        fprintf(stderr, "Error opening result file.\n");
        exit(1);
    }

    fseek(buffer, (long) one.offset, SEEK_SET);
    fwrite(tableToWrite, sizeof(char), one.tableSize, buffer);
    fclose(buffer);

    printf("File 1 size %d, offset %d, width %d, height %d, table size %d, bytes %d\n\n", one.size, one.offset, one.width
            ,one.height, one.tableSize, one.widthInBytes);
    printf("File 2 size %d, offset %d, width %d, height %d, table size %d, bytes %d\n\n", two.size, two.offset, two.width
            ,two.height, two.tableSize, two.widthInBytes);


    int height = START_HEIGHT;
    int width  = START_WIDTH;




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

    display = al_create_display(one.width, one.height);
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

    image = al_load_bitmap(RESULT_FILENAME);
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
                    if(height + two.height +5 <= one.height)
                    {
                        redraw = true;
                        height += 5;
                    }
                    break;

                case ALLEGRO_KEY_DOWN:
                    if(height -5 >= 0)
                    {
                        redraw = true;
                        height -=5;
                    }
                    break;

                case ALLEGRO_KEY_LEFT:
                    if(width -5 >= 0)
                    {
                        redraw = true;
                        width -=5;
                    }
                    break;

                case ALLEGRO_KEY_RIGHT:
                    if(width +two.width + 5 <= one.width)
                    {
                        redraw = true;
                        width +=5;
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
            for(;i<one.tableSize;++i)
                tableToWrite[i] = onlyPixelsFromBig[i];
            blend(width, height, &one, &two, onlyPixelsFromBig, onlyPixelsFromSmall, tableToWrite);

            if((buffer = fopen(RESULT_FILENAME, "rb+")) == NULL)
            {
                fprintf(stderr, "Error opening result file.\n");
                exit(1);
            }

            fseek(buffer, (long) one.offset, SEEK_SET);
            fwrite(tableToWrite, sizeof(char), one.tableSize, buffer);
            fclose(buffer);

            image = al_load_bitmap(RESULT_FILENAME);
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

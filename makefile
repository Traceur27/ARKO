CFLAGS=
LDFLAGS=-L/usr/lib -lallegro -lallegro_image
INCLUDE=-I. -I/usr/include/allegro5

MAIN=main.o
FUNC=blend.o

all: blend

clean:
		rm -rf *.o main pokoloruj

blend: $(FUNC) $(MAIN)
		cc -o $@ $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)
main.o: main.c
		cc -c $^
%.o: %.s
	nasm -f elf64 $<

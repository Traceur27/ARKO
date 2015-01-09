CFLAGS=
LDFLAGS=-L/usr/lib -lallegro -lallegro_image
INCLUDE=-I. -I/usr/include/allegro5

OBJS=main.o
OBJ=pokoloruj.o

all: out

clean:
		rm -rf *.o main pokoloruj

out: $(OBJS) $(OBJ)
		cc -o $@ $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)
main.o: main.c
		cc -c $^
%.o: %.s
	nasm -f elf $<

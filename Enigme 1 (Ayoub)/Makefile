CC      = gcc
CFLAGS  = -g -I/usr/include/SDL -I/usr/include/SDL/SDL_ttf
LDLIBS  = -lSDL -lSDL_image -lSDL_ttf -lX11

all: ex

ex: main.o fonctions.o
	$(CC) main.o fonctions.o -o ex $(LDLIBS)

main.o: main.c fonctions.h
	$(CC) $(CFLAGS) -c main.c

fonctions.o: fonctions.c fonctions.h
	$(CC) $(CFLAGS) -c fonctions.c

clean:
	rm -f ex main.o fonctions.o


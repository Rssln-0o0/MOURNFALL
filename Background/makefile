prog:main.o background.o background.h main.h
	gcc main.o background.o  -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lm -o main -g 
background.o:background.c
	gcc -c background.c -g
main.o:main.c
	gcc -c main.c -g 

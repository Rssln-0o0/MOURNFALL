test: global.o
	gcc global.o -o test -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -g

main.o: principale_source.c
	gcc -c principale_source.c

clean:
	rm -f *.o test
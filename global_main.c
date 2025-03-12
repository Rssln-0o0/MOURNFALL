#include "head.h"
#include "char_source.c"
#include "charg_source.c"
#include "op.c"
#include "principale_source.c"
#include "quiz_source.c"
#include "score_source.c"

int main()
{
    printf("Initializing SDL...\n");
    if (initSDL() < 0 || initTTF() < 0 || initMix() < 0)
        return -1;

    printf("Setting video mode...\n");
    SDL_Surface* screen = SDL_SetVideoMode(1920, 1080, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (!screen)
    {
        printf("Error creating window : %s\n", SDL_GetError());
        return -1;
    }

    printf("Loading font...\n");
    TTF_Font* font = loadFont("text/blue.ttf", 35);
    if (!font)
    {
        printf("Error loading font\n");
        SDL_Quit();
        return -1;
    }

    printf("Loading background music...\n");
    Mix_Music* bgMusic = loadMusic("music/muse_principale_main.wav");
    if (bgMusic)
    {
        Mix_PlayMusic(bgMusic, -1);
    }
    else
    {
        printf("Menu theme failed to load\n");
    }

    displayMenuPrinc(screen, font, bgMusic);

    printf("Cleaning up and exiting...\n");
    cleanUpPrinc(NULL, NULL, font, bgMusic);
    SDL_Quit();
    printf("Exiting..");
    return 0;
}
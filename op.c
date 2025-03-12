#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include "head.h"

Mix_Music *bgMusic = NULL;
int fullscreen = 0;
SDL_Surface *screen = NULL;

SDL_Surface* loadImageOp(const char* filename)
{
    SDL_Surface* image = IMG_Load(filename);
    if (!image)
    {
        printf("Erreur de chargement de l'image (%s) : %s\n", filename, IMG_GetError());
    }
    return image;
}

TTF_Font* loadFontOp(const char* filename, int size)
{
    TTF_Font* font = TTF_OpenFont(filename, size);
    if (!font)
    {
        printf("Erreur de chargement de la police (%s) : %s\n", filename, TTF_GetError());
    }
    return font;
}

Mix_Music* loadMusicOp(const char* filename)
{
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music)
    {
        printf("Erreur de chargement de la musique (%s) : %s\n", filename, Mix_GetError());
    }
    return music;
}

void displayMenuOp(SDL_Surface* screen, TTF_Font* font, Mix_Music* bgMusic, Mix_Chunk* hoverSound, Mix_Chunk* clickSound)
{
    
    SDL_Surface* background = loadImageOp("img/op/bg1.png");
    if (!background) return; // Check if image loaded

    SDL_Surface* elements_image[ELEMENT] = 
    {
        loadImageOp("img/op/apply.png"),
        loadImageOp("img/op/return.png"),
        loadImageOp("img/op/on.png"),
        loadImageOp("img/op/off.png"),
        loadImageOp("img/op/sound1.png"),
        loadImageOp("img/op/sound2.png"),
        loadImageOp("img/op/sound3.png"),
        loadImageOp("img/op/sound4.png"),
        loadImageOp("img/op/sound6.png")
    };

    SDL_Rect positions[ELEMENT] = {
        {1700, 900, 32, 32},  // Apply button
        {100, 900, 32, 32},  // Return button
        {750, 300, 32, 32},  // Fullscreen ON
        {750, 600, 32, 32},  // Sound 
        {750, 600, 32, 32},  // Sound 2
        {750, 600, 32, 32},  // Sound 3
        {750, 600, 32, 32},  // Sound 4
        {750, 600, 32, 32}   // Sound 6
    };

    SDL_Rect positions2[ELEMENT2] = 
    {
        {600, 625, 32, 32},  // Mute
        {1200, 625, 32, 32},  // Loud
    };

    struct {
        char* text;
        SDL_Rect pos;
    } text_labels[] = {
        {"Screen Type", {850, 225, 32, 32}},   // Label for fullscreen toggle
        {"Sound", {900, 550, 32, 32}},         // Label for sound options
        {"Windowed", {535, 325, 32, 32}},      // Windowed mode text
        {"Fullscreen", {1180, 325, 32, 32}},    // Fullscreen mode text
        {"Low", {600, 625, 32, 32}},          // Low
        {"Loud", {1200, 625, 32, 32}},          // Loud
    };

    SDL_Color textcolor = {255,255,255}; // white

    int running = 1;
    SDL_Event event;
    int isFullscreen = 0; // 0 = windowed, 1 = fullscreen
    int soundLevel = 0;   // sound level (0 to 5)
    int lastHovered = -1;
    int lastclick = -1;

    while (running)
    {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, screen, NULL); // Draw background

        SDL_BlitSurface(elements_image[0], NULL, screen, &positions[0]); // Apply
        SDL_BlitSurface(elements_image[1], NULL, screen, &positions[1]); // Return
        SDL_BlitSurface(elements_image[3 - isFullscreen], NULL, screen, &positions[2]); // Fullscreen toggle
        SDL_BlitSurface(elements_image[4 + soundLevel], NULL, screen, &positions[4]); // Sound level

        for (int i = 0; i < sizeof(text_labels) / sizeof(text_labels[0]); i++)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text_labels[i].text, textcolor);
        SDL_BlitSurface(textSurface, NULL, screen, &text_labels[i].pos);
        SDL_FreeSurface(textSurface);
    }

    SDL_Flip(screen);

    while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_MOUSEMOTION)
            {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < 2; i++) // Only check buttons (Apply & Return)
                {
                    if (x >= positions[i].x && x < positions[i].x + elements_image[i]->w &&
                        y >= positions[i].y && y < positions[i].y + elements_image[i]->h)
                    {
                        if (lastHovered != i) {
                            Mix_PlayChannel(-1, hoverSound, 0); // Play hover sound
                            lastHovered = i;
                        }
                    }
                }
            }            

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                // Check if click is on Fullscreen Toggle
                if (x >= positions[2].x && x < positions[2].x + elements_image[2]->w &&
                    y >= positions[2].y && y < positions[2].y + elements_image[2]->h)
                {
                    isFullscreen = !isFullscreen;
                    screen = SDL_SetVideoMode(1920, 1080, 32, isFullscreen ? SDL_FULLSCREEN : SDL_SWSURFACE);
                }

                // Check if click is on Sound Level
                if (x >= positions2[1].x && x < positions2[1].x + elements_image[4]->w &&
                    y >= positions2[1].y && y < positions2[1].y + elements_image[4]->h)
                {
                    if (soundLevel < 4) 
                    {
                        soundLevel++;
                    }
                    int volumeLevels[5] = {32, 64, 96, 128, MIX_MAX_VOLUME}; 
                    Mix_VolumeMusic(volumeLevels[soundLevel]);
                }

                if (x >= positions2[0].x && x < positions2[0].x + elements_image[4]->w &&
                    y >= positions2[0].y && y < positions2[0].y + elements_image[4]->h)
                {
                    if (soundLevel > 0) 
                    {
                        soundLevel--;
                    }

                    int volumeLevels[5] = {32, 64, 96, 128, MIX_MAX_VOLUME};
                    Mix_VolumeMusic(volumeLevels[soundLevel]);
                }

                // Check if clicked Return (Exit settings)
                if (x >= positions[1].x && x < positions[1].x + elements_image[1]->w &&
                    y >= positions[1].y && y < positions[1].y + elements_image[1]->h)
                {
                    running = 0; // Exit settings
                }
            }
        }
    }
    
for (int i = 0; i < ELEMENT; i++)
        SDL_FreeSurface(elements_image[i]);
}

void mp(SDL_Surface* screen)
{
    SDL_Surface* mpImage = loadImageOp("img/op/mp.png");
    if (!mpImage) return;

    SDL_BlitSurface(mpImage, NULL, screen, NULL);
    SDL_Flip(screen);
    SDL_Delay(2000); // Display for 2 seconds
    SDL_FreeSurface(mpImage);
}

void displayHistory(SDL_Surface* screen) // HISTORY NOT RELATED TO THE OPTION
{
    SDL_Rect positions[ELEMENT] = {
        {1700, 900, 32, 32},  // UNUSED - SCRAP
        {100, 900, 32, 32}  // Return button
    };

    SDL_Surface* elements_image[ELEMENT] = 
    {
        loadImageOp("img/op/apply.png"),
        loadImageOp("img/op/return.png")
    };

    SDL_Surface* hisImage = loadImageOp("img/op/his_new.png");
    if (!hisImage)
    return;

    int running = 1;
    SDL_Event event;

    SDL_BlitSurface(hisImage, NULL, screen, NULL);
    SDL_BlitSurface(elements_image[1], NULL, screen, &positions[1]); // Return
    SDL_Flip(screen);
    SDL_Delay(60000);


}

GameState currentState  = MAIN_MENU;

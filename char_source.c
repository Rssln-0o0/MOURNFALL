#include "head.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>

void loadButtons(Menu *menu) {
    const char *buttonFiles[BUTTON_COUNT] = {"play.jpg", "settings.jpg", "score.jpg", "History.jpg", "quit.jpg"};
    const char *buttonHoverFiles[BUTTON_COUNT] = {"play1.jpg", "settings1.jpg", "score1.jpg", "History1.jpg", "quit1.jpg"};
    
    int buttonY = 200;
    for (int i = 0; i < BUTTON_COUNT; i++) {
        menu->buttons[i].imageNormal = IMG_Load(buttonFiles[i]);
        menu->buttons[i].imageHover = IMG_Load(buttonHoverFiles[i]);
        menu->buttons[i].position.x = (SCREEN_WIDTH - menu->buttons[i].imageNormal->w) / 2;
        menu->buttons[i].position.y = buttonY;
        buttonY += 80;
    }
}

void initMenu(Menu *menu) {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    menu->background = IMG_Load("background.jpg");
    menu->music = Mix_LoadMUS("mainmenu.mp3");
    menu->hoverSound = Mix_LoadWAV("mouseclick.wav");
    menu->font = TTF_OpenFont("Amatic-Bold.ttf", 28);
    menu->textColor = (SDL_Color){255, 255, 255};
    menu->titleText = TTF_RenderText_Solid(menu->font, "My Game Menu", menu->textColor);
    menu->selectedButton = -1;

    Mix_PlayMusic(menu->music, -1);
    loadButtons(menu);
}

void renderMenuPlayer(SDL_Surface *screen, Menu *menu) {
    SDL_BlitSurface(menu->background, NULL, screen, NULL);
    SDL_Rect titlePos = {SCREEN_WIDTH / 2 - menu->titleText->w / 2, 50};
    SDL_BlitSurface(menu->titleText, NULL, screen, &titlePos);
    
    for (int i = 0; i < BUTTON_COUNT; i++) {
        SDL_Surface *currentImage = (i == menu->selectedButton) ? menu->buttons[i].imageHover : menu->buttons[i].imageNormal;
        SDL_BlitSurface(currentImage, NULL, screen, &menu->buttons[i].position);
    }
    SDL_Flip(screen);
}

void handleEventsPlayer(SDL_Event event, int *running, Menu *menu) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = 0;
        } else if (event.type == SDL_MOUSEMOTION) {
            menu->selectedButton = -1;
            for (int i = 0; i < BUTTON_COUNT; i++) {
                if (event.motion.x >= menu->buttons[i].position.x &&
                    event.motion.x <= menu->buttons[i].position.x + menu->buttons[i].imageNormal->w &&
                    event.motion.y >= menu->buttons[i].position.y &&
                    event.motion.y <= menu->buttons[i].position.y + menu->buttons[i].imageNormal->h) {
                    if (menu->selectedButton != i) {
                        menu->selectedButton = i;
                        Mix_PlayChannel(-1, menu->hoverSound, 0);
                    }
                }
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (menu->selectedButton == 4) {
                *running = 0;
            }
        }
    }
}

void cleanupMenu(Menu *menu) {
    SDL_FreeSurface(menu->background);
    SDL_FreeSurface(menu->titleText);
    for (int i = 0; i < BUTTON_COUNT; i++) {
        SDL_FreeSurface(menu->buttons[i].imageNormal);
        SDL_FreeSurface(menu->buttons[i].imageHover);
    }
    Mix_FreeMusic(menu->music);
    Mix_FreeChunk(menu->hoverSound);
    TTF_CloseFont(menu->font);
    SDL_Quit();
}
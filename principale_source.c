#include "head.h"
#include <stdio.h>

int initSDLPrinc()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Echec d'initialisation de SDL : %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

int initTTFPrinc()
{
    if (TTF_Init() == -1)
    {
        printf("Echec d'initialisation de TTF : %s\n", TTF_GetError());
        return -1;
    }
    return 0;
}

int initMixPrinc()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        printf("Echec d'initialisation de Mix : %s\n", Mix_GetError());
        return -1;
    }
    return 0;
}

SDL_Surface* loadImagePrinc(const char* filename)
{
    printf("Loading image: %s\n", filename);
    SDL_Surface* image = IMG_Load(filename);
    if (!image)
    {
        printf("Erreur de chargement de l'image (%s) : %s\n", filename, IMG_GetError());
    }
    return image;
}

TTF_Font* loadFontPrinc(const char* filename, int size)
{
    printf("Loading font: %s\n", filename);
    TTF_Font* font = TTF_OpenFont(filename, size);
    if (!font)
    {
        printf("Erreur de chargement de la police (%s) : %s\n", filename, TTF_GetError());
    }
    return font;
}

Mix_Music* loadMusicPrinc(const char* filename)
{
    printf("Loading music: %s\n", filename);
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music)
    {
        printf("Erreur de chargement de la musique (%s) : %s\n", filename, Mix_GetError());
    }
    return music;
}

void displayMenuPrinc(SDL_Surface* screen, TTF_Font* font, Mix_Music* bgMusic)
{
    printf("Entering displayMenuPrinc...\n");

    SDL_Color textColor = {255, 255, 255};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Chrysalis", textColor);
    if (!textSurface)
    {
        printf("Erreur de rendu du texte : %s\n", TTF_GetError());
        return;
    }

    SDL_Rect textPosition = {340, 450, 0, 0};

    SDL_Surface* defaultBackground = loadImage("img/principale/main_bg.png");
    if (!defaultBackground)
    {
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Surface* hoverBackgrounds[ITEM_COUNT] = {
        loadImagePrinc("img/principale/bg_play.png"), 
        loadImagePrinc("img/principale/bg_options.png"),
        loadImagePrinc("img/principale/bg_score.png"), 
        loadImagePrinc("img/principale/bg_story.png"),
        loadImagePrinc("img/principale/bg_quit.png")
    };

    Mix_Chunk* hoverSounds[ITEM_COUNT] = {NULL};
    const char* soundPaths[ITEM_COUNT] = 
    {
        "sounds/muse_principale_sounds/hover_1.wav", 
        "sounds/muse_principale_sounds/hover_2.wav", 
        "sounds/muse_principale_sounds/hover_3.wav", 
        "sounds/muse_principale_sounds/hover_4.wav",
        "sounds/muse_principale_sounds/hover_5.wav"
    };

    for (int i = 0; i < ITEM_COUNT; i++) {
        hoverSounds[i] = Mix_LoadWAV(soundPaths[i]);
        if (!hoverSounds[i]) {
            printf("Failed to load sound %s: %s\n", soundPaths[i], Mix_GetError());
            for (int j = 0; j < i; j++) {
                Mix_FreeChunk(hoverSounds[j]);
                SDL_FreeSurface(hoverBackgrounds[j]);
            }
            SDL_FreeSurface(defaultBackground);
            SDL_FreeSurface(textSurface);
            return;
        }
    }

    MenuItemPrinc menuItems[ITEM_COUNT] = {
        {{574, 745, 400, 350}, "Jouer", hoverBackgrounds[0], hoverSounds[0], NULL},
        {{314, 900, 185, 180}, "Options", hoverBackgrounds[1], hoverSounds[1], NULL},
        {{1420, 840, 235, 140}, "Meilleures Scores", hoverBackgrounds[2], hoverSounds[2], NULL},
        {{980, 116, 400, 370}, "Histoire", hoverBackgrounds[3], hoverSounds[3], NULL},
        {{1760, 720, 200, 200}, "Quitter", hoverBackgrounds[4], hoverSounds[4], NULL}
    };

    int menuRunning = 1;
    int running = 1;
    SDL_Event event;
    int lastHovered = -1;
    int hovered = -1;

    MENU* menuStruct_Charg;

    Mix_Chunk* hoverSound = Mix_LoadWAV("sounds/muse_op_hover.wav");
    Mix_Chunk* clickSound = Mix_LoadWAV("sounds/muse_op_select.wav");
    Mix_Music* bg_save_Music;

    Menu *menuStruct_Player;
    menuStruct_Player = malloc(sizeof(MENU));
    SDL_Surface *fenetre=NULL;

    while (menuRunning)
    {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(defaultBackground, NULL, screen, NULL);
        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                menuRunning = 0;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                hovered = -1;
                for (int i = 0; i < ITEM_COUNT; i++)
                {
                    if (event.motion.x >= menuItems[i].area.x && event.motion.x < menuItems[i].area.x + menuItems[i].area.w &&
                        event.motion.y >= menuItems[i].area.y && event.motion.y < menuItems[i].area.y + menuItems[i].area.h)
                    {
                        hovered = i;
                        break;
                    }
                }
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && hovered != -1)
            {
                switch (hovered)
                {
                    case 0:

                    system("/home/vboxuser/Documents/Mourn/save.exe");
                    
                    break;

                    case 1:

                    displayMenuOp(screen, font, bgMusic, hoverSound, clickSound); // WORKING

                    break;

                    case 2:

                    system("/home/vboxuser/Documents/Mourn/score.exe");

                    break;
                    
                    case 3:

                    displayHistory(screen); // RESPONSIBLE FOR HISTORY IN MENU, THE FUNCTION IS LOCATED IN op.c FOR MORE EDITING OR CHANGING

                    break;


                    case 4:
                    menuRunning = 0; 
                    break;
                }
            }

            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
		    menuRunning = 0;
		    break;
                    case SDLK_j:
		    system("/home/vboxuser/Documents/Mourn/save.exe");
		    break;
                    case SDLK_o:
		    displayMenuOp(screen, font, bgMusic, hoverSound, clickSound);
		    break;
                    case SDLK_m:
		    system("/home/vboxuser/Documents/Mourn/score.exe");
		    break;
		    case SDLK_h:
                    displayHistory(screen);
                    default:
			break;
                }
            }
        }

        if (hovered != -1)
        {
            SDL_BlitSurface(menuItems[hovered].background, NULL, screen, NULL);
            SDL_Surface* hoverTextSurface = TTF_RenderText_Solid(font, menuItems[hovered].label, (SDL_Color){0, 0, 0});
            if (hoverTextSurface)
            {
                SDL_Rect textPos = {menuItems[hovered].area.x, menuItems[hovered].area.y - 30, 0, 0};
                SDL_BlitSurface(hoverTextSurface, NULL, screen, &textPos);
                SDL_FreeSurface(hoverTextSurface);
            }

            if (hovered != lastHovered)
            {
                Mix_PlayChannel(-1, menuItems[hovered].hoverSound, 0);
            }
            lastHovered = hovered;
        }
        else
        {
            lastHovered = -1;
        }

        SDL_Flip(screen);
    }

    if (textSurface) 
    {
        SDL_FreeSurface(textSurface);
        textSurface = NULL;
    }

    if (defaultBackground) 
    {
        SDL_FreeSurface(defaultBackground);
        defaultBackground = NULL;
    }

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (hoverBackgrounds[i]) 
        {
            SDL_FreeSurface(hoverBackgrounds[i]);
            hoverBackgrounds[i] = NULL;
        }
    }

    for (int i = 0; i < ITEM_COUNT; i++) {
        if (hoverSounds[i]) {
            Mix_FreeChunk(hoverSounds[i]);
            hoverSounds[i] = NULL;
        }
    }
}

void cleanUpPrinc(SDL_Surface* image, SDL_Surface* text, TTF_Font* font, Mix_Music* music) 
{
    if (music) Mix_FreeMusic(music);
    if (image) SDL_FreeSurface(image);
    if (text) SDL_FreeSurface(text);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

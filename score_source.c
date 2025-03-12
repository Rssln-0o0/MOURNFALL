#include "head.h"

int init_game(SDL_Surface** ecran, SDL_Surface** background, SDL_Surface** oui, SDL_Surface** non, SDL_Surface** charger, SDL_Surface** partie,SDL_Surface** score1 , SDL_Surface** score2,  Mix_Music** musique, Mix_Chunk** son, Mix_Chunk** click, TTF_Font** police) {

    *background = IMG_Load("img/score/background.png");
    *oui = IMG_Load("img/score/oui.png");
    *non = IMG_Load("img/score/non.png");
    *charger = IMG_Load("img/score/charger.png");
    *partie = IMG_Load("img/score/partie.png");
    *score1 = IMG_Load("img/score/score1.png");
    *score2 = IMG_Load("img/score/score2.png");

    if (!(*background) || !(*oui) || !(*non) || !(*charger) || !(*partie) || !(*score2) || !(*score2)) return 0;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    *musique = Mix_LoadMUS("music/muse_score_main.mp3.mp3");
    *son = Mix_LoadWAV("sounds/muse_score_hover.wav");
    *click = Mix_LoadWAV("sounds/muse_score_select.wav");

    Mix_VolumeMusic(40); // Réduction du volume de la musique de fond
    Mix_VolumeChunk(*click, 100); // Augmenter le volume du son de clic

    return 1;
}

void handle_input(SDL_Event* event, int* quitter, SDL_Rect* posoui, SDL_Rect* posnon, SDL_Rect* poscharger,SDL_Rect* posscore1 , SDL_Rect* posscore2 ,  int* currentPage, Mix_Chunk* son, Mix_Chunk* click, Mix_Music** musique) {
    switch (event->type) {
        case SDL_QUIT:
            *quitter = 1;
            break;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE) *quitter = 1;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT) {
                if (*currentPage == 1 && event->button.x >= posoui->x && event->button.x <= posoui->x + 200 &&
                    event->button.y >= posoui->y && event->button.y <= posoui->y + 100) {
                    *currentPage = 2;
                    Mix_PlayChannel(-1, click, 0);
                    transition_fade(SDL_GetVideoSurface());
                    Mix_PlayMusic(*musique, -1); // Jouer la musique en boucle
                } else if (*currentPage == 2 && event->button.x >= posnon->x && event->button.x <= posnon->x + 200 &&
                           event->button.y >= posnon->y && event->button.y <= posnon->y + 100) {
                    Mix_PlayChannel(-1, click, 0);
                    *quitter = 1;
                } else if (*currentPage == 2 && event->button.x >= poscharger->x && event->button.x <= poscharger->x + 200 &&
                           event->button.y >= poscharger->y && event->button.y <= poscharger->y + 100) {
                    Mix_PlayChannel(-1, click, 0);
                    *currentPage = 1;
                    transition_fade(SDL_GetVideoSurface());
                }
            }
            break;
    }
}

void render_game(SDL_Surface* ecran, SDL_Surface* background, SDL_Surface* oui, SDL_Surface* non, SDL_Surface* charger, SDL_Surface* partie,SDL_Surface* score1 ,SDL_Surface* score2 ,   SDL_Rect* posoui, SDL_Rect* posnon, SDL_Rect* poscharger,
  SDL_Rect* posscore1,SDL_Rect* posscore2,   int currentPage,TTF_Font** police) {
    SDL_BlitSurface(background, NULL, ecran, NULL);

    if (currentPage == 1) {
        SDL_BlitSurface(oui, NULL, ecran, posoui);
        SDL_BlitSurface(partie, NULL, ecran, poscharger);

        SDL_Color textColor = {255, 255, 255}; // Texte en blanc
        SDL_Surface* textSurface = TTF_RenderText_Solid(*police, "Entrer votre pseudo", textColor);
        if (textSurface) {
            SDL_Rect textPos = {200,200}; // Position du texte, à ajuster selon tes besoins
            SDL_BlitSurface(textSurface, NULL, ecran, &textPos);
            SDL_FreeSurface(textSurface);
        }



    } else {
        SDL_BlitSurface(non, NULL, ecran, posnon);
        SDL_BlitSurface(charger, NULL, ecran, poscharger);
        SDL_BlitSurface(score1, NULL, ecran, posscore1);
        SDL_BlitSurface(score2, NULL, ecran, posscore2);



        SDL_Color textColor = {255, 0, 255}; // Texte en blanc
        SDL_Surface* textSurface = TTF_RenderText_Solid(*police, "Listes des meilleurs scores !", textColor);
        if (textSurface) {
            SDL_Rect textPos = {200,200}; // Position du texte, à ajuster selon tes besoins
            SDL_BlitSurface(textSurface, NULL, ecran, &textPos);
            SDL_FreeSurface(textSurface);
        }


    }

    SDL_Flip(ecran);
}

void transition_fade(SDL_Surface* ecran) {
    SDL_Surface* fade = SDL_CreateRGBSurface(SDL_SWSURFACE, 1980, 1080, 32, 0, 0, 0, 0);
    SDL_FillRect(fade, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    for (int alpha = 0; alpha <= 255; alpha += 10) {
        SDL_SetAlpha(fade, SDL_SRCALPHA, alpha);
        SDL_BlitSurface(fade, NULL, ecran, NULL);
        SDL_Flip(ecran);
        SDL_Delay(10);
    }
    SDL_FreeSurface(fade);
}

void cleanupScore(SDL_Surface* background, SDL_Surface* oui, SDL_Surface* non, SDL_Surface* charger, SDL_Surface* partie,SDL_Surface* score1 , SDL_Surface* score2,  Mix_Music* musique, Mix_Chunk* son, Mix_Chunk* click) {
    SDL_FreeSurface(background);
    SDL_FreeSurface(oui);
    SDL_FreeSurface(non);
    SDL_FreeSurface(charger);
    SDL_FreeSurface(partie);
    SDL_FreeSurface(score1);
    SDL_FreeSurface(score2);
    Mix_FreeMusic(musique);
    Mix_FreeChunk(son);
    Mix_FreeChunk(click);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void displayScoreMenu(SDL_Surface* screen, TTF_Font* font) {
    SDL_Surface* background = NULL, *oui = NULL, *non = NULL, *charger = NULL, *partie = NULL, *score1 = NULL, *score2 = NULL;
    Mix_Music* musique = NULL;
    Mix_Chunk* son = NULL, *click = NULL;
    TTF_Font* police = NULL;

    // Initialize the score menu
    if (!init_game(&screen, &background, &oui, &non, &charger, &partie, &score1, &score2, &musique, &son, &click, &police)) {
        printf("Failed to initialize score menu.\n");
        return;
    }

    int quitter = 0; 
    int currentPage = 1;

    SDL_Event event;
    SDL_Rect posoui = {300, 600}, posnon = {100, 500}, poscharger = {100, 600}, posscore1 = {100, 400}, posscore2 = {100, 300};

    while (!quitter) {
        while (SDL_PollEvent(&event)) {
            handle_input(&event, &quitter, &posoui, &posnon, &poscharger, &posscore1, &posscore2, &currentPage, son, click, &musique);
        }

        // Render the score menu
        render_game(screen, background, oui, non, charger, partie, score1, score2, &posoui, &posnon, &poscharger, &posscore1, &posscore2, currentPage, &police);
        SDL_Flip(screen);
        SDL_Delay(16);
    }

    // Clean up resources
    cleanupScore(background, oui, non, charger, partie, score1, score2, musique, son, click);
}



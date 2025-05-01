// main.c
#include "background.h"
#include <SDL/SDL_ttf.h>
#include <stdio.h>



int main() {
    SDL_Surface *ecran, *perso, *perso1, *guideImage;
    SDL_Event event;
    SDL_Rect pos  = {0,200,0,0};
    SDL_Rect pos1 = {400,200,0,0};
    SDL_Rect guidePos = {100,50,0,0};
    background bg, bg1;
    GameTime game_time;
    TTF_Font *font;
    SDL_Color textColor = {255,255,255};
    Mix_Music *musique = NULL;
    int running = 1,
        partage = 0,
        showGuide = 0,
        toggled = 0;

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    font = TTF_OpenFont("arial.ttf",24);
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);  // Mode normal 800x600
    SDL_EnableKeyRepeat(1,1);

    // Chargement assets
    perso      = IMG_Load("background/0.png");
    perso1     = IMG_Load("background/0.png");
    guideImage = IMG_Load("background/guide2.png");
    if (!guideImage) fprintf(stderr,"Erreur guide2.png : %s\n",SDL_GetError());

    // chrono + fond
    game_time.start_time = SDL_GetTicks();
    initback(&bg, musique);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if      (event.type == SDL_QUIT) running = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                  case SDLK_p:
                    if (partage) {
                        // Si déjà en mode partage, revenir à mode normal
                        ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);  // Revenir en mode normal
                        partage = 0;  // Désactiver le mode partage
                    } else {
                        // Si en mode normal, passer en mode partage
                        initpartage(&bg, &bg1, musique);
                        partage = 1;
                        ecran = SDL_SetVideoMode(800, 800, 32, SDL_HWSURFACE);  // Passer en mode partage
                    }
                    break;
                  case SDLK_RIGHT: scrolling(&bg, &pos, 10, 0); break;
                  case SDLK_LEFT:  scrolling(&bg, &pos, 10, 1); break;
                  case SDLK_UP:    scrolling(&bg, &pos, 10, 2); break;
                  case SDLK_DOWN:  scrolling(&bg, &pos, 10, 3); break;
                  case SDLK_d:
                    if (partage) scrolling(&bg1, &pos1, 10, 0);
                    break;
                  case SDLK_q:
                    if (partage) scrolling(&bg1, &pos1, 10, 1);
                    break;
                  case SDLK_z:
                    if (partage) scrolling(&bg1, &pos1, 10, 2);
                    break;
                  case SDLK_s:
                    if (partage) scrolling(&bg1, &pos1, 10, 3);
                    break;
                  case SDLK_g:
                    showGuide = !showGuide; toggled = 1;
                    break;
                }
            }
        }

        // guide auto 10s
        if ((SDL_GetTicks()-game_time.start_time <= 10000) && !toggled)
            showGuide = 1;
        else if (!toggled)
            showGuide = 0;

        // on nettoie l'écran
        SDL_FillRect(ecran, NULL, 0);

        // clamp chaque joueur dans son “view”
        if (!partage) {
            clamp_player(&pos,  &bg,  perso,  800, 600);  // Mode normal 800x600
        } else {
            clamp_player(&pos,  &bg,  perso,  800, 400);  // Mode partage haut
            clamp_player(&pos1, &bg1, perso1, 800, 400);  // Mode partage bas
        }

        // rendu
        if (!partage) {
            SDL_Rect src = { -bg.pos.x, -bg.pos.y, 800, 600 };
            SDL_Rect dst = { 0, 0, 800, 600 };
            SDL_BlitSurface(bg.image, &src, ecran, &dst);
            SDL_BlitSurface(perso, NULL, ecran, &pos);
        } else {
            // haut
            SDL_Rect src = { -bg.pos.x,  -bg.pos.y, 800, 400 };
            SDL_Rect dst = { 0, 0, 800, 400 };
            SDL_BlitSurface(bg.image, &src, ecran, &dst);
            SDL_BlitSurface(perso, NULL, ecran, &pos);
            // bas
            src = (SDL_Rect){ -bg1.pos.x, -bg1.pos.y, 800,400 };
            dst = (SDL_Rect){ 0,400,800,400 };
            SDL_BlitSurface(bg1.image, &src, ecran, &dst);
            SDL_Rect p2 = pos1; p2.y += 400;
            SDL_BlitSurface(perso1, NULL, ecran, &p2);
        }

        if (showGuide)
            SDL_BlitSurface(guideImage, NULL, ecran, &guidePos);
        afficher_temps(ecran, font, &game_time, textColor);

        SDL_Flip(ecran);
        SDL_Delay(16);
    }

    // cleanup
    SDL_FreeSurface(perso);
    SDL_FreeSurface(perso1);
    SDL_FreeSurface(guideImage);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}


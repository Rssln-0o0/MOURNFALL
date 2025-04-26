#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void init_Player(Player* player, const char* idle_frames[], int x, int y) {
    for (int i = 0; i < 4; i++) {
        player->Frames[i] = IMG_Load(idle_frames[i]);
        if (!player->Frames[i]) {
            printf("Erreur chargement frame %d : %s\n", i, SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    player->CurrentFrame = 0;
    player->State = 0; // IDLE BY DEFAULT
    player->Direction = 1; // RIGHT BY DEFAULT
    player->PlayerPos.x = x;
    player->PlayerPos.y = y;
    player->LifeBar = 3;
    player->Score = 0;
    player->Speed_x = 0;
    player->Speed_y = 0;
    player->Jumping = false;
}

void afficher_Player(SDL_Surface* screen, Player* player) {
    SDL_BlitSurface(player->Frames[player->CurrentFrame], NULL, screen, &player->PlayerPos);
}

void deplacer_Player(Player* player, SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    player->Speed_x = -5;
                    player->Direction = -1;
                    player->State = 1; // Marcher
                    break;
                case SDLK_RIGHT:
                    player->Speed_x = 5;
                    player->Direction = 1;
                    player->State = 1; // Marcher
                    break;
                case SDLK_SPACE:
                    if (!player->Jumping) {
                        player->Speed_y = -15;
                        player->Jumping = true;
                        player->State = 3; // Sauter
                    }
                    break;
                case SDLK_LSHIFT:
                    player->Speed_x *= 2;
                    player->State = 2; // Courir
                    break;
                case SDLK_a:
                    player->State = 4; // Attaquer
                    break;
            }
            break;

        case SDL_KEYUP:
            player->Speed_x = 0;
            player->State = 0; // Idle
            break;
    }

    player->PlayerPos.x += player->Speed_x;

    if (player->Jumping) {
        player->Speed_y += 1; // Gravité
        player->PlayerPos.y += player->Speed_y;

        if (player->PlayerPos.y >= 300) { // Collision avec le sol fictif
            player->PlayerPos.y = 300;
            player->Speed_y = 0;
            player->Jumping = false;
            player->State = 0; // Retour à l'état idle
        }
    }
}

void deplacer_Player2(Player* player, SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_j: // Reculer
                    player->Speed_x = -5;
                    player->Direction = -1;
                    player->State = 1; // Marcher
                    break;
                case SDLK_l: // Avancer
                    player->Speed_x = 5;
                    player->Direction = 1;
                    player->State = 1; // Marcher
                    break;
                case SDLK_i: // Sauter
                    if (!player->Jumping) {
                        player->Speed_y = -15;
                        player->Jumping = true;
                        player->State = 3; // Sauter
                    }
                    break;
                case SDLK_k: // Courir
                    player->Speed_x *= 2;
                    player->State = 2; // Courir
                    break;
                case SDLK_n: // Attaquer
                    player->State = 4; // Attaquer
                    break;
            }
            break;

        case SDL_KEYUP:
            player->Speed_x = 0;
            player->State = 0; // Idle
            break;
    }

    player->PlayerPos.x += player->Speed_x;

    if (player->Jumping) {
        player->Speed_y += 1; // Gravité
        player->PlayerPos.y += player->Speed_y;

        if (player->PlayerPos.y >= 300) { // Collision avec le sol fictif
            player->PlayerPos.y = 300;
            player->Speed_y = 0;
            player->Jumping = false;
            player->State = 0; // Retour à l'état idle
        }
    }
}

void animer_Player(Player* player) {
    player->CurrentFrame++;
    if (player->CurrentFrame >= 4) {
        player->CurrentFrame = 0;
    }
}

void afficher_score_vies(SDL_Surface* ecran, TTF_Font* font, Player* player) {
    SDL_Color blanc = {255, 255, 255};
    char texte[50];
    sprintf(texte, "Vies: %d | Score: %d", player->LifeBar, player->Score);
    SDL_Surface* surfaceTexte = TTF_RenderText_Solid(font, texte, blanc);

    SDL_Rect positionTexte = {10, 10};
    SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);
    SDL_FreeSurface(surfaceTexte);
}

void liberer_Player(Player* player) {
    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(player->Frames[i]);
    }
}

void init_Background(Background* bg, const char* chemin_image) {
    bg->BackgroundImage = IMG_Load(chemin_image);
    if (!bg->BackgroundImage) {
        printf("Erreur chargement background : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void afficher_Background(SDL_Surface* screen, Background* bg) {
    SDL_BlitSurface(bg->BackgroundImage, NULL, screen, NULL);
}

void liberer_Background(Background* bg) {
    SDL_FreeSurface(bg->BackgroundImage);
}

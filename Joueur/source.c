#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void init_Player(Player* player, const char* idle_frames[], int x, int y)
{
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

void init_jump(Player* player, float startX, float startY, float endX, float peakY)
{
    player->JumpStartX = startX;
    player->JumpEndX = endX;
    player->JumpProgress = 0.0f;
    player->Jumping = true;
    
    float x1 = startX;
    float x2 = endX;
    float x_peak = (x1 + x2) / 2.0f;
    float y1 = startY;
    float y2 = startY;
    float y_peak = peakY;

    player->JumpC = y1;
    player->JumpA = (y_peak - y1) / ((x_peak - x1) * (x_peak - x1));
    player->JumpB = -2 * player->JumpA * x_peak;
}

void update_jump(Player* player, float deltaTime)
{
    if (!player->Jumping)
        return;
    //progression
    player->JumpProgress += deltaTime;
    //retour idle
    if (player->JumpProgress >= 1.0f)
    {
        player->JumpProgress = 1.0f;
        player->Jumping = false;
        player->State = 0;
        return;
    }
    //nouvelle pos.
    float t = player->JumpProgress;
    float x = player->JumpStartX + t * (player->JumpEndX - player->JumpStartX);
    float y = player->JumpA * x * x + player->JumpB * x + player->JumpC;
    //màj pos.
    player->PlayerPos.x = (int)x;
    player->PlayerPos.y = (int)y;
}

void afficher_Player(SDL_Surface* screen, Player* player)
{
    SDL_BlitSurface(player->Frames[player->CurrentFrame], NULL, screen, &player->PlayerPos);
}

void deplacer_Player(Player* player, SDL_Event event, float deltaTime)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    player->Speed_x = -5;
                    player->Direction = -1;
                    player->State = 1; //marcher (dir. gauche)
                    break;
                case SDLK_RIGHT:
                    player->Speed_x = 5;
                    player->Direction = 1;
                    player->State = 1; //marcher (dir. droite)
                    break;
                case SDLK_SPACE:
                    if (!player->Jumping)
                    {
                        init_jump(player, player->PlayerPos.x, player->PlayerPos.y, player->PlayerPos.x + 150, player->PlayerPos.y - 100);
                        player->State = 3; //jump
                    }
                    break;
                case SDLK_LSHIFT:
                    player->Speed_x *= 2;
                    player->State = 2; //courir
                    break;
                case SDLK_a:
                    player->State = 4; //attaquer
                    break;
            }
            break;

        case SDL_KEYUP:
            player->Speed_x = 0;
            player->State = 0; // idle
            break;
    }

    player->PlayerPos.x += player->Speed_x;

    update_jump(player, deltaTime);
}

void deplacer_Player2(Player* player, SDL_Event event, float deltaTime)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_j:
                    player->Speed_x = -5;
                    player->Direction = -1;
                    player->State = 1; //marcher (dir. gauche)
                    break;
                case SDLK_l:
                    player->Speed_x = 5;
                    player->Direction = 1;
                    player->State = 1; //marcher (dir. droite)
                    break;
                case SDLK_i:
                    if (!player->Jumping)
                    {
                        player->Speed_y = -15;
                        init_jump(player, player->PlayerPos.x, player->PlayerPos.y, player->PlayerPos.x + 150, player->PlayerPos.y - 100);
                        player->State = 3; //jump
                    }
                    break;
                case SDLK_k:
                    player->Speed_x *= 2;
                    player->State = 2; //courir
                    break;
                case SDLK_n:
                    player->State = 4; //attaquer
                    break;
            }
            break;

        case SDL_KEYUP:
            player->Speed_x = 0;
            player->State = 0; //idle
            break;
    }

    player->PlayerPos.x += player->Speed_x;

    update_jump(player, deltaTime);
}

void animer_Player(Player* player)
{
    player->CurrentFrame++;
    if (player->CurrentFrame >= 4) {
        player->CurrentFrame = 0;
    }
}

void afficher_score_vies(SDL_Surface* ecran, TTF_Font* font, Player* player) //to-edit-later
{
    SDL_Color blanc = {255, 255, 255};
    char texte[50];
    sprintf(texte, "Vies: %d | Score: %d", player->LifeBar, player->Score);
    SDL_Surface* surfaceTexte = TTF_RenderText_Solid(font, texte, blanc);

    SDL_Rect positionTexte = {10, 10};
    SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);
    SDL_FreeSurface(surfaceTexte);
}

void liberer_Player(Player* player)
{
    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(player->Frames[i]);
    }
}

void init_Background(Background* bg, const char* chemin_image) //draft
{
    bg->BackgroundImage = IMG_Load(chemin_image);
    if (!bg->BackgroundImage) {
        printf("Erreur chargement background : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void afficher_Background(SDL_Surface* screen, Background* bg)
{
    SDL_BlitSurface(bg->BackgroundImage, NULL, screen, NULL);
}

void liberer_Background(Background* bg)
{
    SDL_FreeSurface(bg->BackgroundImage);
}

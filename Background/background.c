// background.c
#include "background.h"
#include <stdio.h>

void initback(background *bg, Mix_Music *musique) {
    bg->image = IMG_Load("background/partage.png");
    if (!bg->image)
        fprintf(stderr, "Erreur chargement lvl1.png : %s\n", IMG_GetError());
    bg->pos.x = 0;  
    bg->pos.y = 0;
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    musique = Mix_LoadMUS("musique.mp3");
    Mix_PlayMusic(musique, -1);
}

void initpartage(background *bg1, background *bg2, Mix_Music *musique) {
    bg1->image = IMG_Load("background/partage.png");
    bg2->image = IMG_Load("background/partage.png");
    if (!bg1->image || !bg2->image)
        fprintf(stderr, "Erreur chargement partage.png : %s\n", IMG_GetError());
    // On remet les deux calques à 0,0 pour bien couvrir chaque moitié
    bg1->pos.x = 0; bg1->pos.y = 0;
    bg2->pos.x = 0; bg2->pos.y = 0;
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    musique = Mix_LoadMUS("musique.mp3");
    Mix_PlayMusic(musique, -1);
}

void afficher_back(background bg, SDL_Surface *ecran) {
    SDL_BlitSurface(bg.image, NULL, ecran, &bg.pos);
}

void scrolling(background *bg, SDL_Rect *pos, int vitesse, int d) {
    // d = 0 droite, 1 gauche, 2 haut, 3 bas
    if (d == 0) {
        if (pos->x > 100 && bg->pos.x > -(bg->image->w - 800)) {
            bg->pos.x  -= vitesse;
            pos->x     -= vitesse;
        } else {
            pos->x    += vitesse;
        }
    }
    else if (d == 1) {
        if (pos->x < 700 && bg->pos.x < 0) {
            bg->pos.x  += vitesse;
            pos->x     += vitesse;
        } else {
            pos->x    -= vitesse;
        }
    }
    else if (d == 2) {
        if (pos->y > 100 && bg->pos.y > -(bg->image->h - 400)) {
            bg->pos.y  -= vitesse;
            pos->y     -= vitesse;
        } else {
            pos->y    -= vitesse;
        }
    }
    else if (d == 3) {
        if (pos->y < 300 && bg->pos.y < 0) {
            bg->pos.y  += vitesse;
            pos->y     += vitesse;
        } else {
            pos->y    += vitesse;
        }
    }
}

// Affiche le chrono (en secondes)
void afficher_temps(SDL_Surface *ecran,
                    TTF_Font *font,
                    GameTime *game_time,
                    SDL_Color color) {
    game_time->elapsed_time =
        (SDL_GetTicks() - game_time->start_time) / 1000;
    char str[20];
    sprintf(str, "Time: %d s", game_time->elapsed_time);
    SDL_Surface *surf = TTF_RenderText_Solid(font, str, color);
    SDL_Rect p = {10,10,0,0};
    SDL_BlitSurface(surf, NULL, ecran, &p);
    SDL_FreeSurface(surf);
}

// Empêche le joueur de sortir du fond
void clamp_player(SDL_Rect *pos,
                  background *bg,
                  SDL_Surface *player_surf,
                  int view_w,
                  int view_h)
{
    int spriteW  = player_surf->w;
    int spriteH  = player_surf->h;
    // coordonnées dans le "monde"
    int worldX = -bg->pos.x + pos->x;
    int worldY = -bg->pos.y + pos->y;
    // clamp monde
    if (worldX < 0) worldX = 0;
    if (worldY < 0) worldY = 0;
    if (worldX > bg->image->w - spriteW)
        worldX = bg->image->w - spriteW;
    if (worldY > bg->image->h - spriteH)
        worldY = bg->image->h - spriteH;
    // reconvertir en coords écran
    pos->x = worldX + bg->pos.x;
    pos->y = worldY + bg->pos.y;
    // clamp écran (au cas où)
    if (pos->x < 0) pos->x = 0;
    if (pos->y < 0) pos->y = 0;
    if (pos->x > view_w - spriteW)  pos->x = view_w - spriteW;
    if (pos->y > view_h - spriteH)  pos->y = view_h - spriteH;
}

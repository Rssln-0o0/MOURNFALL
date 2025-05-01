// background.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>


// Un calque de fond (image + offset caméra)
typedef struct {
    SDL_Surface *image;
    SDL_Rect pos;
} background;

// Structure chrono
typedef struct {
    Uint32 start_time;
    Uint32 elapsed_time;
} GameTime;

// Initialise un fond simple + lancement musique
void initback(background *bg, Mix_Music *musique);
// Initialise deux fonds partage + musique
void initpartage(background *bg1, background *bg2, Mix_Music *musique);
// Affiche tout le calque (sans découpage)
void afficher_back(background bg, SDL_Surface *ecran);
// Scroll manuel (d=0 droite,1 gauche,2 haut,3 bas)
void scrolling(background *bg, SDL_Rect *pos, int vitesse, int d);
void clamp_player(SDL_Rect *pos,background *bg,SDL_Surface *player_surf,int view_w, int view_h);

void afficher_temps(SDL_Surface *ecran, TTF_Font *font,GameTime *game_time, SDL_Color color);

#endif // BACKGROUND_H


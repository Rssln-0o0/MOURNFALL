#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdbool.h>
#include <SDL/SDL_ttf.h>

typedef struct {
    SDL_Surface* Frames[4];
    SDL_Rect PlayerPos;
    int CurrentFrame, LifeBar, Score;
    int State;              // 0 = idle, 1 = marcher, 2 = courir, 3 = sauter, 4 = attaquer
    int Direction;          // Gauche = -1, Droite = 1              
    int Speed_x, Speed_y;       
    bool Jumping;
    float JumpA, JumpB, JumpC;
    float JumpStartX, JumpEndX, JumpProgress;
} Player;

typedef struct {
    SDL_Surface* BackgroundImage;
} Background;


void init_Player(Player* player, const char* idle_frames[], int x, int y);
void init_jump(Player* player, float startX, float startY, float endX, float peakY);
void update_jump(Player* player, float deltaTime);
void afficher_Player(SDL_Surface* screen, Player* player);
void deplacer_Player(Player* player, SDL_Event event);
void deplacer_Player2(Player* player, SDL_Event event);
void animer_Player(Player* player);
void afficher_score_vies(SDL_Surface* ecran, TTF_Font* font, Player* player);
void liberer_Player(Player* player);
void init_Background(Background* bg, const char* chemin_image);
void afficher_Background(SDL_Surface* screen, Background* bg);
void liberer_Background(Background* bg);

#endif // HEADER_H_INCLUDED

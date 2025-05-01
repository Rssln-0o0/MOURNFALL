#ifndef FONCTIONS_H
#define FONCTIONS_H



#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h> // <-- Ajoutez cette ligne pour reconnaître TTF_Font

// ... le reste de vos définitions ...


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Taille du niveau complet (par exemple, 2000x1500 ou 1920x1080)
#define LEVEL_WIDTH 1920
#define LEVEL_HEIGHT 1080

// Taille de la mini map (par exemple, 200x150)
#define MINIMAP_WIDTH 200
#define MINIMAP_HEIGHT 150
#define MINIMAP_MARGIN 10

// Structure regroupant les données du niveau et de la mini map
typedef struct {
    SDL_Surface *level;    // Image du niveau complet (level1.png ou level2.png)
    SDL_Surface *minimap;  // Surface de la mini map générée dynamiquement
    SDL_Rect camera;       // Zone visible du niveau (la "caméra")
} LevelData;

// Structure pour le joueur
typedef struct {
    SDL_Rect pos;          // Position et taille du joueur dans le niveau
} Player;

// Prototypes des fonctions relatives au niveau, à la mini map et à la caméra
void initLevel(LevelData *ld, const char *levelFilename);
void freeLevel(LevelData *ld);
void updateCamera(LevelData *ld, Player *player);
void displayLevel(SDL_Surface *screen, LevelData *ld, Player *player);
void generateMinimap(LevelData *ld);
void displayMinimap(SDL_Surface *screen, LevelData *ld, Player *player);
void changeLevel(LevelData *ld, const char *levelFilename);

// Fonctions de collision
int collisionBB(SDL_Rect a, SDL_Rect b);
int collisionPixelPerfect(SDL_Surface *object, SDL_Surface *platform, SDL_Rect posObject, SDL_Rect posPlatform);

// Animation d'arrière-plan en cas de collision
void animationBackground(SDL_Surface *screen, int collisionFlag);

// Sauvegarde et chargement (par exemple, de la position de la caméra)
void saveGame(LevelData *ld);
void loadGame(LevelData *ld);

// Affichage de la fenêtre guide (instructions) sur l'écran
void showGuide(SDL_Surface *screen, TTF_Font *font);

// Sous-menu meilleurs scores : saisie du nom du joueur et sauvegarde du score
void showBestScores(int score);

#endif // FONCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "fonctions.h"

/* Implémentation de SDL_IntersectRect si nécessaire */
#ifndef SDL_IntersectRect
int SDL_IntersectRect(const SDL_Rect *A, const SDL_Rect *B, SDL_Rect *result) {
    int Ax1 = A->x, Ay1 = A->y, Ax2 = A->x + A->w, Ay2 = A->y + A->h;
    int Bx1 = B->x, By1 = B->y, Bx2 = B->x + B->w, By2 = B->y + B->h;
    if (Ax2 < Bx1 || Ax1 > Bx2 || Ay2 < By1 || Ay1 > By2)
        return 0;
    if (result) {
        result->x = (Ax1 > Bx1) ? Ax1 : Bx1;
        result->y = (Ay1 > By1) ? Ay1 : By1;
        result->w = ((Ax2 < Bx2) ? Ax2 : Bx2) - result->x;
        result->h = ((Ay2 < By2) ? Ay2 : By2) - result->y;
    }
    return 1;
}
#endif

// Génère dynamiquement la mini map à partir du niveau
void generateMinimap(LevelData *ld) {
    ld->minimap = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT,
                                        ld->level->format->BitsPerPixel,
                                        ld->level->format->Rmask,
                                        ld->level->format->Gmask,
                                        ld->level->format->Bmask,
                                        ld->level->format->Amask);
    if (!ld->minimap) {
        fprintf(stderr, "Erreur de création de la mini map: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect src = {0, 0, ld->level->w, ld->level->h};
    SDL_Rect dst = {0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT};
    SDL_SoftStretch(ld->level, &src, ld->minimap, &dst);
}

// Charge le niveau à partir d'un fichier et génère la mini map
void initLevel(LevelData *ld, const char *levelFilename) {
    ld->level = IMG_Load(levelFilename);
    if (!ld->level) {
        fprintf(stderr, "Erreur de chargement de %s: %s\n", levelFilename, IMG_GetError());
        exit(1);
    }
    generateMinimap(ld);
    ld->camera.x = 0;
    ld->camera.y = 0;
    ld->camera.w = SCREEN_WIDTH;
    ld->camera.h = SCREEN_HEIGHT;
}

// Libère les ressources du niveau
void freeLevel(LevelData *ld) {
    if (ld->level) {
        SDL_FreeSurface(ld->level);
        ld->level = NULL;
    }
    if (ld->minimap) {
        SDL_FreeSurface(ld->minimap);
        ld->minimap = NULL;
    }
}

// Met à jour la caméra pour centrer sur le joueur
void updateCamera(LevelData *ld, Player *player) {
    ld->camera.x = player->pos.x + player->pos.w / 2 - SCREEN_WIDTH / 2;
    ld->camera.y = player->pos.y + player->pos.h / 2 - SCREEN_HEIGHT / 2;
    if (ld->camera.x < 0) ld->camera.x = 0;
    if (ld->camera.y < 0) ld->camera.y = 0;
    if (ld->camera.x + SCREEN_WIDTH > LEVEL_WIDTH)
        ld->camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
    if (ld->camera.y + SCREEN_HEIGHT > LEVEL_HEIGHT)
        ld->camera.y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}

// Affiche la portion du niveau correspondant à la caméra
void displayLevel(SDL_Surface *screen, LevelData *ld, Player *player) {
    SDL_BlitSurface(ld->level, &ld->camera, screen, NULL);
}

// Affiche la mini map et le repère du joueur
void displayMinimap(SDL_Surface *screen, LevelData *ld, Player *player) {
    SDL_Rect miniPos;
    miniPos.x = SCREEN_WIDTH - MINIMAP_WIDTH - MINIMAP_MARGIN;
    miniPos.y = MINIMAP_MARGIN;
    miniPos.w = MINIMAP_WIDTH;
    miniPos.h = MINIMAP_HEIGHT;
    
    SDL_BlitSurface(ld->minimap, NULL, screen, &miniPos);
    
    float scaleX = (float)MINIMAP_WIDTH / LEVEL_WIDTH;
    float scaleY = (float)MINIMAP_HEIGHT / LEVEL_HEIGHT;
    
    int markerX = miniPos.x + (int)(player->pos.x * scaleX);
    int markerY = miniPos.y + (int)(player->pos.y * scaleY);
    SDL_Rect marker = { markerX - 2, markerY - 2, 5, 5 };
    SDL_FillRect(screen, &marker, SDL_MapRGB(screen->format, 255, 0, 0));
}

// Change le niveau et regénère la mini map à partir du nouveau niveau
void changeLevel(LevelData *ld, const char *levelFilename) {
    SDL_Surface *newLevel = IMG_Load(levelFilename);
    if (!newLevel) {
        fprintf(stderr, "Erreur de chargement de %s: %s\n", levelFilename, IMG_GetError());
        return;
    }
    SDL_FreeSurface(ld->level);
    ld->level = newLevel;
    if (ld->minimap) SDL_FreeSurface(ld->minimap);
    generateMinimap(ld);
    printf("Niveau changé : %s\n", levelFilename);
}

// Collision par bounding box
int collisionBB(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h)
        return 0;
    return 1;
}

// Fonction auxiliaire pour obtenir un pixel (pour collision pixel-perfect)
Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0]<<16 | p[1]<<8 | p[2];
            else
                return p[0] | p[1]<<8 | p[2]<<16;
        case 4: return *(Uint32 *)p;
        default: return 0;
    }
}

// Collision pixel-perfect (simplifiée) : vérifie dans la zone d'intersection si des pixels opaques se chevauchent
int collisionPixelPerfect(SDL_Surface *object, SDL_Surface *platform, SDL_Rect posObject, SDL_Rect posPlatform) {
    SDL_Rect intersection;
    if (!SDL_IntersectRect(&posObject, &posPlatform, &intersection))
        return 0;
    for (int y = intersection.y; y < intersection.y + intersection.h; y++) {
        for (int x = intersection.x; x < intersection.x + intersection.w; x++) {
            int objX = x - posObject.x;
            int objY = y - posObject.y;
            int platX = x - posPlatform.x;
            int platY = y - posPlatform.y;
            if (object && platform) {
                Uint32 pixelObj = get_pixel(object, objX, objY);
                Uint32 pixelPlat = get_pixel(platform, platX, platY);
                if (pixelObj != 0 && pixelPlat != 0)
                    return 1;
            }
        }
    }
    return 0;
}

// Animation d'arrière-plan : flash rouge pendant 50ms toutes les 2 secondes en cas de collision
void animationBackground(SDL_Surface *screen, int collisionFlag) {
    if (collisionFlag) {
        Uint32 ticks = SDL_GetTicks();
        if (ticks % 2000 < 50) {
            SDL_Rect rect = {0, 0, screen->w, screen->h};
            SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 0, 0));
        }
    }
}

// Sauvegarde de la position de la caméra (exemple)
void saveGame(LevelData *ld) {
    FILE *file = fopen("sauvegarde.txt", "w");
    if (!file) {
        fprintf(stderr, "Erreur d'ouverture de sauvegarde.txt pour sauvegarde.\n");
        return;
    }
    fprintf(file, "camera_x=%d\n", ld->camera.x);
    fprintf(file, "camera_y=%d\n", ld->camera.y);
    fclose(file);
    printf("Sauvegarde effectuée dans sauvegarde.txt.\n");
}

// Chargement de la position de la caméra depuis un fichier
void loadGame(LevelData *ld) {
    FILE *file = fopen("sauvegarde.txt", "r");
    if (!file) {
        fprintf(stderr, "Erreur d'ouverture de sauvegarde.txt pour chargement.\n");
        return;
    }
    int x, y;
    if (fscanf(file, "camera_x=%d\ncamera_y=%d\n", &x, &y) == 2) {
        ld->camera.x = x;
        ld->camera.y = y;
        printf("Chargement réussi: camera_x=%d, camera_y=%d\n", x, y);
    } else {
        fprintf(stderr, "Format incorrect dans sauvegarde.txt.\n");
    }
    fclose(file);
}

// Affiche une fenêtre guide avec des instructions (pendant 3 secondes)
void showGuide(SDL_Surface *screen, TTF_Font *font) {
    // Créer une surface pour le guide
    const char *guideText = "Guide: Fleches pour deplacer, P pour changer niveau, Q pour quitter et sauvegarder score";
    SDL_Color yellow = {255, 255, 0};
    SDL_Surface *guideSurface = TTF_RenderText_Solid(font, guideText, yellow);
    if (!guideSurface) {
        fprintf(stderr, "Erreur TTF_RenderText_Solid (guide): %s\n", TTF_GetError());
        return;
    }
    // Centrer le guide à l'écran
    SDL_Rect pos;
    pos.x = (SCREEN_WIDTH - guideSurface->w) / 2;
    pos.y = (SCREEN_HEIGHT - guideSurface->h) / 2;
    // Afficher le guide par-dessus l'écran (semi-transparent)
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(guideSurface, NULL, screen, &pos);
    SDL_Flip(screen);
    SDL_Delay(3000); // Affiche pendant 3 secondes
    SDL_FreeSurface(guideSurface);
}

// Sous-menu meilleurs scores : demande le nom du joueur et sauvegarde le score
void showBestScores(int score) {
    char name[50];
    printf("Entrez votre nom pour sauvegarder votre score (%d) : ", score);
    scanf("%49s", name);
    FILE *file = fopen("scores.txt", "a");
    if (file) {
        fprintf(file, "%s %d\n", name, score);
        fclose(file);
        printf("Score sauvegarde!\n");
    } else {
        printf("Erreur lors de l'ouverture de scores.txt\n");
    }
}


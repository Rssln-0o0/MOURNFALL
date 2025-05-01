#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <SDL/SDL.h>
#include <stdbool.h>

/* Structure Enigme : regroupe toutes les surfaces et positions */
typedef struct {
    SDL_Surface* puzzle;            /* image du puzzle complet */
    SDL_Surface* proposals[3];      /* les 3 propositions */
    SDL_Surface* victory;           /* message victoire */
    SDL_Surface* defeat;            /* message défaite */
    SDL_Rect     puzzlePos;         /* où blitter le puzzle */
    SDL_Rect     proposalsPos[3];   /* positions des propositions */
    SDL_Rect     missingSlot;       /* zone trouée dans le puzzle */
    int          correctIndex;      /* indice de la bonne proposition */
} Enigme;

/* Structure DragState : gère le glisser/déposer */
typedef struct {
    int     selectedIndex;  /* indice de la proposition active, -1 si aucune */
    bool    dragging;       /* mode drag actif */
    SDL_Rect pos_piece;     /* position courante de la pièce drag */
    int     offset_x,       /* décalage entre clic et coin */
            offset_y;
} DragState;

/* Chargement d'image via SDL_image + conversion display format */
SDL_Surface* charger_image(const char* nom_fichier);

/* Blit simple d'une surface en (x,y) */
void          afficher_image(SDL_Surface* image, SDL_Surface* ecran, int x, int y);

/* Retourne un entier aléatoire dans [min..max] */
int           generer_nombre_aleatoire(int min, int max);

/* Affiche une image de message (victoire/defaite) centrée pendant 3s */
void          afficher_message_simple(SDL_Surface* ecran, const char* image_message);

/* Callback SDL_AddTimer : anime un rectangle rouge pour le chrono */
Uint32        animer_chrono(Uint32 intervalle, void* param);

#endif


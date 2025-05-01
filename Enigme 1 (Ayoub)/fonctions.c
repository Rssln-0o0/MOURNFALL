#include "fonctions.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

SDL_Surface* charger_image(const char* nom_fichier) {
    SDL_Surface* tmp = IMG_Load(nom_fichier);
    if (!tmp) {
        fprintf(stderr, "Erreur de chargement de '%s' : %s\n",
                nom_fichier, IMG_GetError());
        return NULL;
    }
    SDL_Surface* img = (tmp->format->Amask)
        ? SDL_DisplayFormatAlpha(tmp)
        : SDL_DisplayFormat(tmp);
    SDL_FreeSurface(tmp);
    return img;
}

void afficher_image(SDL_Surface* image, SDL_Surface* ecran, int x, int y) {
    SDL_Rect dst = { x, y, 0, 0 };
    SDL_BlitSurface(image, NULL, ecran, &dst);
}

int generer_nombre_aleatoire(int min, int max) {
    return min + rand() % (max - min + 1);
}

void afficher_message_simple(SDL_Surface* ecran, const char* image_message) {
    SDL_Surface* msg = charger_image(image_message);
    if (!msg) return;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0,0,0));
    int x = (ecran->w - msg->w) / 2;
    int y = (ecran->h - msg->h) / 2;
    afficher_image(msg, ecran, x, y);
    SDL_Flip(ecran);
    SDL_Delay(3000);
    SDL_FreeSurface(msg);
}

Uint32 animer_chrono(Uint32 intervalle, void* param) {
    SDL_Surface* ecran = (SDL_Surface*)param;
    static int width = 0;
    width = (width + 5) % (ecran->w - 20);
    SDL_Rect rect = { 10, 10, width, 20 };
    SDL_FillRect(ecran, &rect, SDL_MapRGB(ecran->format, 255,0,0));
    SDL_Flip(ecran);
    return intervalle;
}


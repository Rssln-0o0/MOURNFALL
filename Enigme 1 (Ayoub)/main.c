#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_image.h>
#include <X11/Xlib.h>   // nécessaire pour XInitThreads() sous SDL1.2
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char* argv[]) {
    // Initialisation multi-thread X11
    if (!XInitThreads()) return 1;
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) return 1;
    if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG|IMG_INIT_JPG)))
        return 1;

    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Enigme Fantasy & Myths", NULL);
    srand((unsigned)time(NULL));

    /* --- Chargement et positionnement de l'énigme --- */
    Enigme E;
    E.puzzle        = charger_image("puzzel.jpg");
    E.proposals[0]  = charger_image("1.png");
    E.proposals[1]  = charger_image("2.png");
    E.proposals[2]  = charger_image("3.png");
    E.victory       = charger_image("v.jpeg");
    E.defeat        = charger_image("d.jpeg");
    E.correctIndex  = generer_nombre_aleatoire(0, 2);
    printf("La bonne proposition est la %d\n", E.correctIndex + 1);

    // Puzzle centré en haut (600px de large idéalement)
    E.puzzlePos.x = (800 - E.puzzle->w) / 2;
    E.puzzlePos.y = 50;
    E.puzzlePos.w = E.puzzle->w;
    E.puzzlePos.h = E.puzzle->h;

    // Définition du "trou" (missing slot) dans le puzzle
    E.missingSlot.w = E.puzzlePos.w / 5;
    E.missingSlot.h = E.puzzlePos.h / 5;
    E.missingSlot.x = E.puzzlePos.x + E.puzzlePos.w / 3;
    E.missingSlot.y = E.puzzlePos.y + E.puzzlePos.h / 3;

    // Positionnement des 3 propositions en bas
    int spacing = 20;
    int totalW = E.proposals[0]->w
               + E.proposals[1]->w
               + E.proposals[2]->w
               + 2 * spacing;
    int sx = (800 - totalW) / 2;
    int py = 600 - E.proposals[0]->h - 50;
    for (int i = 0; i < 3; i++) {
        E.proposalsPos[i].x = sx;
        E.proposalsPos[i].y = py;
        E.proposalsPos[i].w = E.proposals[i]->w;
        E.proposalsPos[i].h = E.proposals[i]->h;
        sx += E.proposals[i]->w + spacing;
    }

    /* --- Initialisation du drag & drop --- */
    DragState D = { .selectedIndex = -1, .dragging = false };

    bool running = true, solved = false;
    SDL_Event ev;
    SDL_AddTimer(100, animer_chrono, screen);

    /* --- Boucle principale --- */
    while (running && !solved) {
        // Effacer l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));

        // 1) Blit du puzzle complet
        afficher_image(E.puzzle, screen,
                       E.puzzlePos.x, E.puzzlePos.y);

        // 2) Perçage du trou
        SDL_FillRect(screen, &E.missingSlot,
                     SDL_MapRGB(screen->format, 0,0,0));

        // 3) Contour rouge du trou
        {
            SDL_Rect o = E.missingSlot, l;
            // Haut
            l = (SDL_Rect){o.x, o.y, o.w, 1};
            SDL_FillRect(screen, &l, SDL_MapRGB(screen->format,255,0,0));
            // Bas
            l = (SDL_Rect){o.x, o.y + o.h - 1, o.w, 1};
            SDL_FillRect(screen, &l, SDL_MapRGB(screen->format,255,0,0));
            // Gauche
            l = (SDL_Rect){o.x, o.y, 1, o.h};
            SDL_FillRect(screen, &l, SDL_MapRGB(screen->format,255,0,0));
            // Droite
            l = (SDL_Rect){o.x + o.w - 1, o.y, 1, o.h};
            SDL_FillRect(screen, &l, SDL_MapRGB(screen->format,255,0,0));
        }

        // 4) Affichage des propositions (sauf celle en cours de drag)
        for (int i = 0; i < 3; i++) {
            if (D.dragging && i == D.selectedIndex) continue;
            afficher_image(E.proposals[i], screen,
                           E.proposalsPos[i].x,
                           E.proposalsPos[i].y);
        }

        // 5) Affichage de la pièce déplacée
        if (D.dragging) {
            afficher_image(E.proposals[D.selectedIndex], screen,
                           D.pos_piece.x, D.pos_piece.y);
        }

        SDL_Flip(screen);

        // Gestion des événements
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT)
                running = false;
            else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            else if (ev.type == SDL_MOUSEBUTTONDOWN
                  && ev.button.button == SDL_BUTTON_LEFT) {
                int mx = ev.button.x, my = ev.button.y;
                if (!D.dragging) {
                    // Vérifier clic sur une proposition
                    for (int i = 0; i < 3; i++) {
                        SDL_Rect r = E.proposalsPos[i];
                        if (mx >= r.x && mx <= r.x + r.w
                        &&  my >= r.y && my <= r.y + r.h) {
                            D.selectedIndex = i;
                            D.dragging      = true;
                            D.pos_piece     = r;
                            D.offset_x      = mx - r.x;
                            D.offset_y      = my - r.y;
                            break;
                        }
                    }
                }
            }
            else if (ev.type == SDL_MOUSEMOTION && D.dragging) {
                D.pos_piece.x = ev.motion.x - D.offset_x;
                D.pos_piece.y = ev.motion.y - D.offset_y;
            }
            else if (ev.type == SDL_MOUSEBUTTONUP
                  && ev.button.button == SDL_BUTTON_LEFT
                  && D.dragging) {
                D.dragging = false;
                int i = D.selectedIndex;
                // mettre à jour la position fixe
                E.proposalsPos[i] = D.pos_piece;
                // si c'est la bonne pièce et déposée dans le trou
                if (i == E.correctIndex) {
                    SDL_Rect ms = E.missingSlot, rp = D.pos_piece;
                    int m = 20;
                    if (abs(rp.x - ms.x) < m
                    &&  abs(rp.y - ms.y) < m)
                        solved = true;
                }
            }
        }

        SDL_Delay(16);
    }

    // Afficher le message final
    if (solved)
        afficher_message_simple(screen, "v.jpeg");
    else
        afficher_message_simple(screen, "d.jpeg");

    // Attente avant fermeture
    bool wait = true;
    while (wait) {
        while (SDL_PollEvent(&ev))
            if (ev.type == SDL_QUIT
             || (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
                wait = false;
        SDL_Delay(16);
    }

    // Libération des ressources
    SDL_FreeSurface(E.puzzle);
    for (int i = 0; i < 3; i++)
        SDL_FreeSurface(E.proposals[i]);
    SDL_FreeSurface(E.victory);
    SDL_FreeSurface(E.defeat);
    IMG_Quit();
    SDL_Quit();
    return 0;
}


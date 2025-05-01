#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include "fonctions.h"

// Fonction utilitaire pour dessiner un contour (pour déboguer les collisions)
void drawRectOutline(SDL_Surface *screen, SDL_Rect rect, int thickness, Uint32 color) {
    SDL_Rect top    = { rect.x, rect.y, rect.w, thickness };
    SDL_Rect bottom = { rect.x, rect.y + rect.h - thickness, rect.w, thickness };
    SDL_Rect left   = { rect.x, rect.y, thickness, rect.h };
    SDL_Rect right  = { rect.x + rect.w - thickness, rect.y, thickness, rect.h };

    SDL_FillRect(screen, &top, color);
    SDL_FillRect(screen, &bottom, color);
    SDL_FillRect(screen, &left, color);
    SDL_FillRect(screen, &right, color);
}

int main(int argc, char *argv[]) {
    /* Initialisation de SDL et TTF */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
         fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
         return 1;
    }
    if (TTF_Init() == -1) {
         fprintf(stderr, "Erreur TTF_Init: %s\n", TTF_GetError());
         SDL_Quit();
         return 1;
    }
    
    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (!screen) {
         fprintf(stderr, "Erreur SDL_SetVideoMode: %s\n", SDL_GetError());
         TTF_Quit();
         SDL_Quit();
         return 1;
    }
    
    // Activer la répétition des touches pour un déplacement continu
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    
    // Charger la police (assurez-vous que "arial.ttf" est présent)
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
         fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
         SDL_FreeSurface(screen);
         TTF_Quit();
         SDL_Quit();
         return 1;
    }
    
    // Charger l'image du joueur (player.png) – taille recommandée : 20x20
    SDL_Surface *playerSurface = IMG_Load("player.jpg");
    if (!playerSurface) {
         printf("Aucun fichier player.png trouvé, le joueur ne sera pas affiché sur la mini map.\n");
    }
    
    LevelData ld;
    // Charge level1.png et génère dynamiquement la mini map
    initLevel(&ld, "1.jpg");
    
    Player player;
    player.pos.x = 100;
    player.pos.y = 100;
    player.pos.w = 20;
    player.pos.h = 20;
    
    // Charger les images des plateformes pour collision
    SDL_Surface *platformFixed = IMG_Load("platform_fixed.jpg");
    SDL_Rect fixedRect = {600, 500, 150, 20};
    SDL_Surface *platformMobile = IMG_Load("platform_mobile.jpg");
    SDL_Rect mobileRect = {1000, 700, 150, 20};
    
    if (!platformFixed) {
         printf("Aucun fichier platform_fixed.png trouvé, utilisation d'un rectangle fixe pour la collision.\n");
    }
    if (!platformMobile) {
         printf("Aucun fichier platform_mobile.png trouvé, utilisation d'un rectangle mobile pour la collision.\n");
    }
    
    Uint32 startTime = SDL_GetTicks();
    int currentLevel = 1;
    int running = 1;
    SDL_Event event;
    
    while (running) {
         while (SDL_PollEvent(&event)) {
              if (event.type == SDL_QUIT)
                   running = 0;
              else if (event.type == SDL_KEYDOWN) {
                   switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                             running = 0;
                             break;
                        case SDLK_UP:
                             player.pos.y -= 5;
                             break;
                        case SDLK_DOWN:
                             player.pos.y += 5;
                             break;
                        case SDLK_LEFT:
                             player.pos.x -= 5;
                             break;
                        case SDLK_RIGHT:
                             player.pos.x += 5;
                             break;
                        case SDLK_g:
                             // Afficher le guide du jeu
                             showGuide(screen, font);
                             break;
                        case SDLK_q:
                             {
                                  // Fin de jeu et sous-menu meilleurs scores
                                  Uint32 finalScore = SDL_GetTicks() - startTime;
                                  showBestScores(finalScore);
                                  running = 0;
                             }
                             break;
                        case SDLK_s:
                             saveGame(&ld);
                             break;
                        case SDLK_l:
                             loadGame(&ld);
                             break;
                        case SDLK_p:
                             // Bascule entre Level1 et Level2
                             if (currentLevel == 1) {
                                  changeLevel(&ld, "2.jpg");
                                  currentLevel = 2;
                                  printf("Changement vers Level 2\n");
                                  ld.camera.x = 0; ld.camera.y = 0;
                             } else {
                                  changeLevel(&ld, "1.jpg");
                                  currentLevel = 1;
                                  printf("Changement vers Level 1\n");
                                  ld.camera.x = 0; ld.camera.y = 0;
                             }
                             break;
                        default:
                             break;
                   }
              }
         }
         
         // Contraindre le joueur dans le niveau
         if (player.pos.x < 0) player.pos.x = 0;
         if (player.pos.y < 0) player.pos.y = 0;
         if (player.pos.x + player.pos.w > LEVEL_WIDTH)
              player.pos.x = LEVEL_WIDTH - player.pos.w;
         if (player.pos.y + player.pos.h > LEVEL_HEIGHT)
              player.pos.y = LEVEL_HEIGHT - player.pos.h;
         
         // Vérifier la collision avec la plateforme fixe
         int collisionFixed = 0;
         if (platformFixed && playerSurface)
              collisionFixed = collisionPixelPerfect(playerSurface, platformFixed, player.pos, fixedRect);
         else
              collisionFixed = collisionBB(player.pos, fixedRect);
         
         // Vérifier la collision avec la plateforme mobile (bounding box)
         int collisionMobile = collisionBB(player.pos, mobileRect);
         int collisionActive = collisionFixed || collisionMobile;
         if (collisionActive) {
              printf("Collision détectée !\n");
         }
         
         // Pour le débogage visuel : dessiner un contour autour de la zone d'intersection pour la plateforme fixe
         SDL_Rect intersectFixed;
         if (SDL_IntersectRect(&player.pos, &fixedRect, &intersectFixed)) {
              SDL_Rect intersectFixedScreen = { intersectFixed.x - ld.camera.x, intersectFixed.y - ld.camera.y, intersectFixed.w, intersectFixed.h };
              drawRectOutline(screen, intersectFixedScreen, 2, SDL_MapRGB(screen->format, 255, 0, 255));
         }
         // Pour la plateforme mobile
         SDL_Rect intersectMobile;
         if (SDL_IntersectRect(&player.pos, &mobileRect, &intersectMobile)) {
              SDL_Rect intersectMobileScreen = { intersectMobile.x - ld.camera.x, intersectMobile.y - ld.camera.y, intersectMobile.w, intersectMobile.h };
              drawRectOutline(screen, intersectMobileScreen, 2, SDL_MapRGB(screen->format, 255, 0, 255));
         }
         
         updateCamera(&ld, &player);
         
         SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
         
         // Afficher le niveau (scrolling)
         displayLevel(screen, &ld, &player);
         
         // Afficher le joueur dans le niveau
         SDL_Rect playerScreen = {
              player.pos.x - ld.camera.x,
              player.pos.y - ld.camera.y,
              player.pos.w,
              player.pos.h
         };
         if (playerSurface) {
              SDL_BlitSurface(playerSurface, NULL, screen, &playerScreen);
         }
         
         // Afficher la plateforme mobile dans le niveau
         SDL_Rect mobileScreen = { mobileRect.x - ld.camera.x, mobileRect.y - ld.camera.y, mobileRect.w, mobileRect.h };
         if (platformMobile)
              SDL_BlitSurface(platformMobile, NULL, screen, &mobileScreen);
         else
              SDL_FillRect(screen, &mobileScreen, SDL_MapRGB(screen->format, 0, 200, 0));
         // La plateforme fixe reste cachée : on ne l'affiche pas dans le niveau
         
         // Afficher la mini map
         displayMinimap(screen, &ld, &player);
         
         // Afficher le joueur sur la mini map
         if (playerSurface) {
              float scaleX = (float)MINIMAP_WIDTH / LEVEL_WIDTH;
              float scaleY = (float)MINIMAP_HEIGHT / LEVEL_HEIGHT;
              int miniX = SCREEN_WIDTH - MINIMAP_WIDTH - MINIMAP_MARGIN;
              int miniY = MINIMAP_MARGIN;
              int miniPlayerX = miniX + (int)(player.pos.x * scaleX);
              int miniPlayerY = miniY + (int)(player.pos.y * scaleY);
              SDL_Rect miniPlayerRect = { miniPlayerX - (playerSurface->w / 2),
                                           miniPlayerY - (playerSurface->h / 2),
                                           playerSurface->w,
                                           playerSurface->h };
              SDL_BlitSurface(playerSurface, NULL, screen, &miniPlayerRect);
         }
         
         // Afficher la plateforme mobile sur la mini map
         {
              float scaleX = (float)MINIMAP_WIDTH / LEVEL_WIDTH;
              float scaleY = (float)MINIMAP_HEIGHT / LEVEL_HEIGHT;
              int miniX = SCREEN_WIDTH - MINIMAP_WIDTH - MINIMAP_MARGIN;
              int miniY = MINIMAP_MARGIN;
              
              // La plateforme fixe est cachée : on ne l'affiche pas sur la mini map
              
              // Afficher la plateforme mobile sur la mini map (couleur bleu clair)
              SDL_Rect mobileMini = {
                  miniX + (int)(mobileRect.x * scaleX),
                  miniY + (int)(mobileRect.y * scaleY),
                  (int)(mobileRect.w * scaleX),
                  (int)(mobileRect.h * scaleY)
              };
              SDL_FillRect(screen, &mobileMini, SDL_MapRGB(screen->format, 100, 200, 255));
         }
         
         // Afficher le temps écoulé en mm:ss
         Uint32 elapsed = SDL_GetTicks() - startTime;
         int minutes = elapsed / 60000;
         int seconds = (elapsed / 1000) % 60;
         char timeText[50];
         sprintf(timeText, "Time: %02d:%02d", minutes, seconds);
         SDL_Color white = {255, 255, 255};
         SDL_Surface *timeSurface = TTF_RenderText_Solid(font, timeText, white);
         if (timeSurface) {
              SDL_Rect timePos = {10, 10, 0, 0};
              SDL_BlitSurface(timeSurface, NULL, screen, &timePos);
              SDL_FreeSurface(timeSurface);
         }
         
         // Animation d'arrière-plan en cas de collision
         animationBackground(screen, collisionActive);
         
         SDL_Flip(screen);
         SDL_Delay(16);  // environ 60 FPS
    }
    
    if (playerSurface)
         SDL_FreeSurface(playerSurface);
    if (platformFixed)
         SDL_FreeSurface(platformFixed);
    if (platformMobile)
         SDL_FreeSurface(platformMobile);
    freeLevel(&ld);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}


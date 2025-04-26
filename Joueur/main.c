#include "header.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur initialisation SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() == -1) {
        printf("Erreur initialisation SDL_ttf : %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface* screen = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE);
    if (!screen) {
        printf("Erreur mode vidéo : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    TTF_Font* font = TTF_OpenFont("Nexa-Heavy.ttf", 35);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    Background bg;
    init_Background(&bg, "background.jpg");

    // PLAYER 1
    const char* idle_frames[] = {
        "idle1.png", "idle2.png", "idle3.png", "idle4.png"
    };
    const char* walk_frames[] = {
        "walk1.png", "walk2.png", "walk3.png", "walk4.png"
    };
    const char* run_frames[] = {
        "run1.png", "run2.png", "run3.png", "run4.png"
    };
    const char* jump_frames[] = {
        "jump1.png", "jump2.png", "jump3.png", "jump4.png"
    };
    const char* attack_frames[] = {
        "attack1.png", "attack2.png", "attack3.png", "attack4.png"
    };

    // PLAYER 2
    const char* player2_idle_frames[] = {
        "player2_idle1.png", "player2_idle2.png", "player2_idle3.png", "player2_idle4.png"
    };
    const char* player2_walk_frames[] = {
        "player2_walk1.png", "player2_walk2.png", "player2_walk3.png", "player2_walk4.png"
    };
    const char* player2_run_frames[] = {
        "player2_run1.png", "player2_run2.png", "player2_run3.png", "player2_run4.png"
    };
    const char* player2_jump_frames[] = {
        "player2_jump1.png", "player2_jump2.png", "player2_jump3.png", "player2_jump4.png"
    };
    const char* player2_attack_frames[] = {
        "player2_attack1.png", "player2_attack2.png", "player2_attack3.png", "player2_attack4.png"
    };

    Player player1;
    init_Player(&player1, idle_frames, 100, 300);

    Player player2;
    bool player2_initialized = false;

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) {
                if (!player2_initialized) {
                    init_Player(&player2, player2_idle_frames, 200, 300);
                    player2_initialized = true;
                }
            }

            deplacer_Player(&player1, event);
            if (player2_initialized) {
                deplacer_Player2(&player2, event);
            }
        }

	    if (player1.State == 0) { // Idle
                for (int i = 0; i < 4; i++) {
                    player1.Frames[i] = IMG_Load(idle_frames[i]);
                }
            } else if (player1.State == 1) { // Marcher
                for (int i = 0; i < 4; i++) {
                    player1.Frames[i] = IMG_Load(walk_frames[i]);
                }
            } else if (player1.State == 2) { // Courir
                for (int i = 0; i < 4; i++) {
                    player1.Frames[i] = IMG_Load(run_frames[i]);
                }
            } else if (player1.State == 3) { // Sauter
                for (int i = 0; i < 4; i++) {
                    player1.Frames[i] = IMG_Load(jump_frames[i]);
                }
            } else if (player1.State == 4) { // Attaquer
                for (int i = 0; i < 4; i++) {
                    player1.Frames[i] = IMG_Load(attack_frames[i]);
                }
            }
            if (player2_initialized) {
            if (player2.State == 0) { // Idle
                for (int i = 0; i < 4; i++) {
                    player2.Frames[i] = IMG_Load(player2_idle_frames[i]);
                }
            } else if (player2.State == 1) { // Marcher
                for (int i = 0; i < 4; i++) {
                    player2.Frames[i] = IMG_Load(player2_walk_frames[i]);
                }
            } else if (player2.State == 2) { // Courir
                for (int i = 0; i < 4; i++) {
                    player2.Frames[i] = IMG_Load(player2_run_frames[i]);
                }
            } else if (player2.State == 3) { // Sauter
                for (int i = 0; i < 4; i++) {
                    player2.Frames[i] = IMG_Load(player2_jump_frames[i]);
                }
            } else if (player2.State == 4) { // Attaquer
                for (int i = 0; i < 4; i++) {
                    player2.Frames[i] = IMG_Load(player2_attack_frames[i]);
                }
            } 
            }
	 
        animer_Player(&player1);
        if (player2_initialized) {
            animer_Player(&player2);
        }

        afficher_Background(screen, &bg);
        afficher_Player(screen, &player1);
        if (player2_initialized) {
            afficher_Player(screen, &player2);
        }
        afficher_score_vies(screen, font, &player1);
        if (player2_initialized) {
            afficher_score_vies(screen, font, &player2);
        }
        SDL_Flip(screen);
    }

    liberer_Background(&bg);
    liberer_Player(&player1);
    if (player2_initialized) {
        liberer_Player(&player2);
    }
    TTF_CloseFont(font);
    SDL_Quit();
    return EXIT_SUCCESS;
}

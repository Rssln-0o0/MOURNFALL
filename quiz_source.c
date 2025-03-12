#include "head.h"

// Load images from files

/* int load_images(SDL_Surface **background_quiz, SDL_Surface **button_quiz, SDL_Surface **buttonHover_quiz,
                SDL_Surface **quiz, SDL_Surface **correct_quiz, SDL_Surface **wrong1_quiz, SDL_Surface **wrong2_quiz,
                SDL_Surface **correctClicked_quiz, SDL_Surface **wrong1Clicked_quiz, SDL_Surface **wrong2Clicked_quiz) {
                
    (*background) = SDL_LoadBMP("img/quiz/quiz_bg.bmp");

    if (*background == NULL) {
        printf("Unable to load background image: %s\n", SDL_GetError());
        return 0;
    }

    (*button_quiz) = SDL_LoadBMP("img/quiz/button.bmp");
    if (*button_quiz == NULL) {
        printf("Unable to load button image: %s\n", SDL_GetError());
        return 0;
    }

    (*buttonHover_quiz) = SDL_LoadBMP("img/quiz/button_hover.bmp");
    if (*buttonHover_quiz == NULL) {
        printf("Unable to load button hover image: %s\n", SDL_GetError());
        return 0;
    }

    /* quiz = SDL_LoadBMP("quiz.bmp");
    if (*quiz == NULL) {
        printf("Unable to load quiz image: %s\n", SDL_GetError());
        return 0;
    }

    (*correct_quiz) = SDL_LoadBMP("img/quiz/correct.bmp");
    if (*correct_quiz == NULL) {
        printf("Unable to load correct button image: %s\n", SDL_GetError());
        return 0;
    }

    (*wrong1_quiz) = SDL_LoadBMP("img/quiz/wrong1.bmp");
    if (*wrong1_quiz == NULL) {
        printf("Unable to load wrong1 button image: %s\n", SDL_GetError());
        return 0;
    }

    (*wrong2_quiz) = SDL_LoadBMP("img/quiz/wrong2.bmp");
    if (*wrong2_quiz == NULL) {
        printf("Unable to load wrong2 button image: %s\n", SDL_GetError());
        return 0;
    }

    (*correctClicked_quiz) = SDL_LoadBMP("img/quiz/correct_clicked.bmp");
    if (*correctClicked_quiz == NULL) {
        printf("Unable to load correct clicked button image: %s\n", SDL_GetError());
        return 0;
    }

    (*wrong1Clicked_quiz) = SDL_LoadBMP("img/quiz/wrong1_clicked.bmp");
    if (*wrong1Clicked_quiz == NULL) {
        printf("Unable to load wrong1 clicked button image: %s\n", SDL_GetError());
        return 0;
    }

    (*wrong2Clicked_quiz) = SDL_LoadBMP("img/quiz/wrong2_clicked.bmp");
    if (*wrong2Clicked_quiz == NULL) {
        printf("Unable to load wrong2 clicked button image: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
} */

void quiz_image_load()
{

SDL_Surface **background_quiz;
SDL_Surface **button_quiz;
SDL_Surface **buttonHover_quiz;
SDL_Surface **quiz;
SDL_Surface **correct_quiz;
SDL_Surface **wrong1_quiz;
SDL_Surface **wrong2_quiz;
SDL_Surface **correctClicked_quiz;
SDL_Surface **wrong1Clicked_quiz;
SDL_Surface **wrong2Clicked_quiz;

*background_quiz = SDL_LoadBMP("img/quiz/quiz_bg.bmp");
*button_quiz = SDL_LoadBMP("img/quiz/button.bmp");
*buttonHover_quiz = SDL_LoadBMP("img/quiz/button_hover.bmp");
*correct_quiz = SDL_LoadBMP("img/quiz/correct.bmp");
*wrong1_quiz = SDL_LoadBMP("img/quiz/wrong1.bmp");
*wrong2_quiz = SDL_LoadBMP("img/quiz/wrong2.bmp");
*correctClicked_quiz = SDL_LoadBMP("img/quiz/correct_clicked.bmp");
*wrong1Clicked_quiz = SDL_LoadBMP("img/quiz/wrong1_clicked.bmp");
*wrong2Clicked_quiz = SDL_LoadBMP("img/quiz/wrong2_clicked.bmp");

}


// Load sound effects
void quiz_music_load(Mix_Chunk **click, Mix_Chunk **correct, Mix_Chunk **wrong)
{
    *click = Mix_LoadWAV("sounds/muse_op_select.wav");
    *correct = Mix_LoadWAV("sounds/muse_quiz_right1.wav");
    *wrong = Mix_LoadWAV("sounds/muse_quiz_wrong.wav");
}


// Apply a surface to another surface at the specified coordinates
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, destination, &offset);
}

void events_quiz()
{
    int quit = 0;
    int showQuiz = 0;
    int button1Clicked = 0;
    int button2Clicked = 0;
    int button3Clicked = 0;
    SDL_Event event;

    Mix_Chunk *clickSound = NULL;
    Mix_Chunk *correctSound = NULL;
    Mix_Chunk *wrongSound = NULL;

    SDL_Surface *correct = NULL;
    SDL_Surface *wrong1 = NULL;
    SDL_Surface *wrong2 = NULL;
    SDL_Surface *correctClicked = NULL;
    SDL_Surface *wrong1Clicked = NULL;
    SDL_Surface *wrong2Clicked = NULL;
    SDL_Surface *buttonHover = NULL;
    SDL_Surface *quiz = NULL;
    SDL_Surface *button = NULL;

    SDL_Surface* source;
    SDL_Surface *destination;

    // Button positions and states
    int buttonX = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
    int buttonY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2;
    int isHovered = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }

            // Check for mouse motion
            if (event.type == SDL_MOUSEMOTION) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                if (!showQuiz) {
                    // Check if mouse is over the main button
                    if (mouseX >= buttonX && mouseX <= buttonX + BUTTON_WIDTH &&
                        mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT) {
                        isHovered = 1;
                    } else {
                        isHovered = 0;
                    }
                }
            }

            // Check for mouse click
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (!showQuiz) {
                    // Check if main button is clicked
                    if (mouseX >= buttonX && mouseX <= buttonX + BUTTON_WIDTH &&
                        mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        showQuiz = 1;
                    }
                } else {
                    // Check if quiz buttons are clicked
                    if (mouseX >= QUIZ_BUTTON_X1 && mouseX <= QUIZ_BUTTON_X1 + QUIZ_BUTTON_WIDTH &&
                        mouseY >= QUIZ_BUTTON_Y && mouseY <= QUIZ_BUTTON_Y + QUIZ_BUTTON_HEIGHT) {
                        Mix_PlayChannel(-1, wrongSound, 0); // Wrong answer
                        button1Clicked = 1;
                    } else if (mouseX >= QUIZ_BUTTON_X2 && mouseX <= QUIZ_BUTTON_X2 + QUIZ_BUTTON_WIDTH &&
                               mouseY >= QUIZ_BUTTON_Y && mouseY <= QUIZ_BUTTON_Y + QUIZ_BUTTON_HEIGHT) {
                        Mix_PlayChannel(-1, correctSound, 0); // Correct answer
                        button2Clicked = 1;
                    } else if (mouseX >= QUIZ_BUTTON_X3 && mouseX <= QUIZ_BUTTON_X3 + QUIZ_BUTTON_WIDTH &&
                               mouseY >= QUIZ_BUTTON_Y && mouseY <= QUIZ_BUTTON_Y + QUIZ_BUTTON_HEIGHT) {
                        Mix_PlayChannel(-1, wrongSound, 0); // Wrong answer
                        button3Clicked = 1;
                    }
                }
            }
        }

        // Clear the screen
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Apply the background
        apply_surface(0, 0, source, screen);

        if (!showQuiz) {
            // Apply the main button (hover or normal)
            if (isHovered) {
                apply_surface(buttonX, buttonY, buttonHover, screen);
            } else {
                apply_surface(buttonX, buttonY, button, screen);
            }
        } else {
            // Apply the quiz screen (centered)
            int quizX = (SCREEN_WIDTH - QUIZ_WIDTH) / 2;
            int quizY = (SCREEN_HEIGHT - QUIZ_HEIGHT) / 2;
            apply_surface(quizX, quizY, quiz, screen);

            // Apply quiz buttons (change if clicked)
            if (button1Clicked) {
                apply_surface(QUIZ_BUTTON_X1, QUIZ_BUTTON_Y, wrong1Clicked, screen);
            } else {
                apply_surface(QUIZ_BUTTON_X1, QUIZ_BUTTON_Y, wrong1, screen);
            }

            if (button2Clicked) {
                apply_surface(QUIZ_BUTTON_X2, QUIZ_BUTTON_Y, correctClicked, screen);
            } else {
                apply_surface(QUIZ_BUTTON_X2, QUIZ_BUTTON_Y, correct, screen);
            }

            if (button3Clicked) {
                apply_surface(QUIZ_BUTTON_X3, QUIZ_BUTTON_Y, wrong2Clicked, screen);
            } else {
                apply_surface(QUIZ_BUTTON_X3, QUIZ_BUTTON_Y, wrong2, screen);
            }
        }

        // Update the screen
        SDL_Flip(screen);int quit = 0;
        int showQuiz = 0;
        int button1Clicked = 0;
        int button2Clicked = 0;
        int button3Clicked = 0;
        SDL_Event event;
    
        // Button positions and states
        int buttonX = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
        int buttonY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2;
        int isHovered = 0;
    
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = 1;
                }
    
                // Check for mouse motion
                if (event.type == SDL_MOUSEMOTION) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
    
                    if (!showQuiz) {
                        // Check if mouse is over the main button
                        if (mouseX >= buttonX && mouseX <= buttonX + BUTTON_WIDTH &&
                            mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT) {
                            isHovered = 1;
                        } else {
                            isHovered = 0;
                        }
                    }
                }
    
                // Check for mouse click
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
    
                    if (!showQuiz) {
                        // Check if main button is clicked
                        if (mouseX >= buttonX && mouseX <= buttonX + BUTTON_WIDTH &&
                            mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT) {
                            Mix_PlayChannel(-1, clickSound, 0);
                            showQuiz = 1;
                        }
                    } else {
                        // Check if quiz buttons are clicked
                        if (mouseX >= QUIZ_BUTTON_X1 && mouseX <= QUIZ_BUTTON_X1 + QUIZ_BUTTON_WIDTH &&
                            mouseY >= QUIZ_BUTTON_Y && mouseY <= QUIZ_BUTTON_Y + QUIZ_BUTTON_HEIGHT) {
                            Mix_PlayChannel(-1, wrongSound, 0); // Wrong answer
                            button1Clicked = 1;
                        } else if (mouseX >= QUIZ_BUTTON_X2 && mouseX <= QUIZ_BUTTON_X2 + QUIZ_BUTTON_WIDTH &&
                                   mouseY >= QUIZ_BUTTON_Y && mouseY <= QUIZ_BUTTON_Y + QUIZ_BUTTON_HEIGHT) {
                            Mix_PlayChannel(-1, correctSound, 0); // Correct answer
                            button2Clicked = 1;
                        } else if (mouseX >= QUIZ_BUTTON_X3 && mouseX <= QUIZ_BUTTON_X3 + QUIZ_BUTTON_WIDTH &&
                                   mouseY >= QUIZ_BUTTON_Y && mouseY <= QUIZ_BUTTON_Y + QUIZ_BUTTON_HEIGHT) {
                            Mix_PlayChannel(-1, wrongSound, 0); // Wrong answer
                            button3Clicked = 1;
                        }
                    }
                }
            }
    
            // Clear the screen
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    
            // Apply the background
            apply_surface(0, 0, source, screen);
    
            if (!showQuiz) {
                // Apply the main button (hover or normal)
                if (isHovered) {
                    apply_surface(buttonX, buttonY, buttonHover, screen);
                } else {
                    apply_surface(buttonX, buttonY, button, screen);
                }
            } else {
                // Apply the quiz screen (centered)
                int quizX = (SCREEN_WIDTH - QUIZ_WIDTH) / 2;
                int quizY = (SCREEN_HEIGHT - QUIZ_HEIGHT) / 2;
                apply_surface(quizX, quizY, quiz, screen);
    
                // Apply quiz buttons (change if clicked)
                if (button1Clicked) {
                    apply_surface(QUIZ_BUTTON_X1, QUIZ_BUTTON_Y, wrong1Clicked, screen);
                } else {
                    apply_surface(QUIZ_BUTTON_X1, QUIZ_BUTTON_Y, wrong1, screen);
                }
    
                if (button2Clicked) {
                    apply_surface(QUIZ_BUTTON_X2, QUIZ_BUTTON_Y, correctClicked, screen);
                } else {
                    apply_surface(QUIZ_BUTTON_X2, QUIZ_BUTTON_Y, correct, screen);
                }
    
                if (button3Clicked) {
                    apply_surface(QUIZ_BUTTON_X3, QUIZ_BUTTON_Y, wrong2Clicked, screen);
                } else {
                    apply_surface(QUIZ_BUTTON_X3, QUIZ_BUTTON_Y, wrong2, screen);
                }
            }
    
            // Update the screen
            SDL_Flip(screen);
        }
    }
}
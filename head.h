// menu.h - Header file

#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SLIDER_WIDTH 200
#define ITEM_COUNT 5
#define SLIDER_HEIGHT 20
#define BUTTON_COUNT 5
#define ELEMENT 12
#define ELEMENT2 3

#define save_menu_images 20
#define save_menu_pos 20


#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

#define QUIZ_WIDTH 400 
#define QUIZ_HEIGHT 300

#define QUIZ_BUTTON_WIDTH 100
#define QUIZ_BUTTON_HEIGHT 40

#define QUIZ_BUTTON_X1 120  // Adjusted for smaller quiz
#define QUIZ_BUTTON_X2 270  // Adjusted for smaller quiz
#define QUIZ_BUTTON_X3 420  // Adjusted for smaller quiz
#define QUIZ_BUTTON_Y 350 


extern SDL_Surface *screen;
extern SDL_Surface *background;
extern SDL_Surface *elements;
extern Mix_Music *bgMusic;
extern SDL_Surface* image;
extern SDL_Surface* text;
extern Mix_Music* music;
extern int fullscreen;
extern int volume;

typedef enum GameState {MAIN_MENU, OPTIONS, SHOW_TEST_IMAGE0} GameState;

typedef struct { // STRUCT CHARGEMENT

	SDL_Surface *BACKGROUND;
	SDL_Surface *ECRANJOUEUR;
	SDL_Surface *NOUV1;
	SDL_Surface *NOUV2;
	SDL_Surface *CHARGER1;
	SDL_Surface *CHARGER2;
	SDL_Surface *OUI1;
	SDL_Surface *OUI2;
	SDL_Surface *NON1;
	SDL_Surface *NON2;
	
	SDL_Rect positionBACKGROUND;
	SDL_Rect positionNOUV;
	SDL_Rect positionCHARGER;
    SDL_Rect positionOUI;
	SDL_Rect positionNON;
	SDL_Rect positionTXT;
	Mix_Chunk *sonbutton;
	int buttonon,GAMEPAGE,changemenu,SELECTEDBUT,sonbref;
	TTF_Font *police;
	SDL_Surface *text;
	SDL_Color white;
	SDL_Rect positionTEXT;
	Mix_Music *music;
	
}MENU;


typedef struct { // MENU PLAYER
    SDL_Surface *imageNormal;
    SDL_Surface *imageHover;
    SDL_Rect position;
} Button;

typedef struct { // MENU PLAYER
    SDL_Surface *background;
    Mix_Music *music;
    Mix_Chunk *hoverSound;
    TTF_Font *font;
    SDL_Color textColor;
    SDL_Surface *titleText;
    Button buttons[BUTTON_COUNT];
    int selectedButton;
}Menu;

typedef struct {
    SDL_Rect area;
    const char* label;
    SDL_Surface* background;
    Mix_Chunk* hoverSound;
    const char* subMenuFile;
}MenuItemPrinc;

void init_menu(); // General


// ************************** MENU PRINCIPALE ***********************************

void displayMenuPrinc(SDL_Surface* screen, TTF_Font* font, Mix_Music* bgMusic);
void cleanUpPrinc(SDL_Surface* image, SDL_Surface* text, TTF_Font* font, Mix_Music* music);


// ************************** MENU CHARGEMENT ***********************************

void initMENU(MENU *menuStruct_Charg); // Menu Chargement
void MenuINPUTS(MENU *menuStruct_Charg,SDL_Event event); // Menu Chargement Inputs
void afficherMENU(MENU *menuStruct_Charg,SDL_Surface *screen); // Menu Chargement Afficher
void sonbref(MENU *menuStruct_Charg); // Menu Chargement somb ??

void displaySaveMenu(SDL_Surface* screen, TTF_Font* font, Mix_Music* bg_save_Music); // Global

// ************************* MENU SETTINGS **************************************

void displayMenuOp(SDL_Surface* screen, TTF_Font* font, Mix_Music* bgMusic, Mix_Chunk* hoverSound, Mix_Chunk* clickSound);
void mp(SDL_Surface* screen);
void displayHistory(SDL_Surface* screen); // HISTORY PART IN THE MENU

// ************************* MENU PLAYER **************************************

void initMenuPlayer(Menu *menu);
void renderMenuPlayer(SDL_Surface *screen, Menu *menu);
void handleEventsPlayer(SDL_Event event, int *running, Menu *menu);
void cleanupPlayer(Menu *menu);
void loadButtonsPlayer(Menu *menu);
void startAmelioration();

// ************************* MENU QUIZ **************************************

void quiz_image_load();
int load_sounds(Mix_Chunk **click, Mix_Chunk **correct, Mix_Chunk **wrong);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);
void events_quiz();

// ************************* MENU SCORE **************************************

int init_game(SDL_Surface** ecran, SDL_Surface** background, SDL_Surface** oui, SDL_Surface** non, SDL_Surface** charger, SDL_Surface** partie,SDL_Surface** score1, SDL_Surface** score2 ,  Mix_Music** musique, Mix_Chunk** son, Mix_Chunk** click, TTF_Font** police);
void handle_input(SDL_Event* event, int* quitter, SDL_Rect* posoui, SDL_Rect* posnon, SDL_Rect* poscharger,SDL_Rect* posscore1 , SDL_Rect* posscore2,  int* currentPage, Mix_Chunk* son, Mix_Chunk* click, Mix_Music** musique);
void render_game(SDL_Surface* ecran, SDL_Surface* background, SDL_Surface* oui, SDL_Surface* non, SDL_Surface* charger, SDL_Surface* partie,SDL_Surface* score1 , SDL_Surface* score2 ,  SDL_Rect* posoui, SDL_Rect* posnon, SDL_Rect* poscharger, SDL_Rect* posscore1,SDL_Rect* posscore2 ,  int currentPage,TTF_Font** police);
void transition_fade(SDL_Surface* ecran);
void cleanupScore(SDL_Surface* background, SDL_Surface* oui, SDL_Surface* non, SDL_Surface* charger, SDL_Surface* partie,SDL_Surface* score1 , SDL_Surface* score2 , Mix_Music* musique, Mix_Chunk* son, Mix_Chunk* click);
void displayScoreMenu(SDL_Surface* screen, TTF_Font* font);

void cleanupGlobal();

#endif


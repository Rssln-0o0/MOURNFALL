#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

// تعريف الثوابت
#define MAX_QUESTIONS 10         // أقصى عدد للأسئلة
#define QUESTION_LENGTH 256      // أقصى طول للسؤال
#define ANSWER_LENGTH 64       // أقصى طول للإجابة
#define NOMBRE_CHOIX 3         // عدد الاختيارات في السؤال

// هيكل يمثل سؤال/لغز
typedef struct {
    char question[QUESTION_LENGTH];    // نص السؤال أو وصف اللغز
    char answers[NOMBRE_CHOIX][ANSWER_LENGTH]; // مصفوفة تخزن الاختيارات (في QCM) أو المعطيات (في Puzzle)
    int correct_index;                 // رقم الفهرس للإجابة الصحيحة (يبدأ من 0)
    char type[10];                     // نوع الإنيغمة ("QCM" أو "Puzzle")
} Enigme;

// هيكل لإدارة مجموعة الأسئلة/الألغاز
typedef struct {
    Enigme enigmes[MAX_QUESTIONS];       // مصفوفة تخزن كل الألغاز المحملة
    int nombre_enigmes_chargees;       // عدد الألغاز اللي تم تحميلها بنجاح
    int dernier_index_pose;            // فهرس آخر لغز تم طرحه باش نتجنبوا التكرار
} GestionnaireEnigmes;

// الدوال الخاصة بإدارة الألغاز
int chargerEnigmes(const char *filename, GestionnaireEnigmes *gestionnaire);
void afficherEnigme(SDL_Surface *ecran, TTF_Font *font, const Enigme *enigme);
int verifierReponse(const Enigme *enigme, int reponse_utilisateur);
Enigme* obtenirEnigmeAleatoire(GestionnaireEnigmes *gestionnaire);
Enigme* obtenirEnigmeAleatoireByType(GestionnaireEnigmes *gestionnaire, const char *type);

#endif


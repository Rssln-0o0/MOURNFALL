#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include "enigme.h"
#include "chronometre.h"

// تعريف الثوابت
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CHRONO_FPS 10 // قداش من إطار نعرضوا في الثانية الواحدة للكرونومتر (يتحكم في سرعة الحركة)

int main(int argc, char *argv[]) {
    // تهيئة SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    // تهيئة SDL_ttf باش نخدموا بالخطوط
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    // تهيئة SDL_image لتحميل الصور (PNG في حالتنا)
    if (!(IMG_Init(IMG_INIT_PNG))) {
        fprintf(stderr, "IMG_Init failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // إنشاء الشاشة
    SDL_Surface *ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        fprintf(stderr, "Screen initialization failed: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_WM_SetCaption("Jeu d'Enigmes", NULL); // عنوان النافذة

    // تحميل الخط اللي باش نستعملوه في النصوص
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24); // افترضنا عندك ملف خط اسمه arial.ttf في نفس الفولدر
    if (!font) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // تهيئة مولد الأرقام العشوائية
    srand(time(NULL));

    // تحميل الألغاز
    GestionnaireEnigmes gestionnaire;
    if (!chargerEnigmes("questions.txt", &gestionnaire)) {
        TTF_CloseFont(font);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // تحميل رسوم الكرونومتر
    ChronometreAnimation chrono;
    // بما إنك قلت إن أسماء التصاور chrono_1.png لين chrono_9.png
    // باش نبعثوا "chrono_" كـ chemin_base و 9 كـ nombre_images
    // و délai بين الإطارات باش يكون على حساب CHRONO_FPS
    if (chargerAnimationChronometre("chrono_", 9, 1000 / CHRONO_FPS, &chrono) == -1) {
        TTF_CloseFont(font);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // تحديد موقع عرض الكرونومتر (عدل الإحداثيات حسب حاجتك)
    SDL_Rect position_chrono = { .x = 500, .y = 50 };

    int continuer = 1; // متغير باش تبقى حلقة اللعبة خدامة لين المستخدم يسكر
    Enigme *enigme_actuelle = obtenirEnigmeAleatoire(&gestionnaire); // نجيبوا أول لغز عشوائي

    while (continuer) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                continuer = 0; // المستخدم ضغط على زر الإغلاق
            }
            // هنا تنجم تزيد معالجة أحداث أخرى (ضغطات على الكلافي، الفأرة...)
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_3 && enigme_actuelle) {
                    int reponse = event.key.keysym.sym - SDLK_0; // نحولوا رقم الزر لرقم الإجابة
                    if (verifierReponse(enigme_actuelle, reponse)) {
                        printf("bravo reponse correcte!\n");
                        enigme_actuelle = obtenirEnigmeAleatoire(&gestionnaire); // نجيبوا لغز جديد
                        demarrerChronometre(&chrono); // نعاودوا نبداو الكرونومتر من الأول مع اللغز الجديد
                    } else {
                        printf("desole reponse fausse..ressayee.\n");
                    }
                }
            }
        }

        // 1. امسح الشاشة (لون أسود كمثال)
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

        // 2. حدث الكرونومتر باش يبدل التصويرة إذا فات الوقت المحدد
        mettreAJourAnimationChronometre(&chrono);

        // 3. اعرض الكرونومتر في المكان اللي حددناه
        afficherAnimationChronometre(ecran, &chrono, position_chrono);

        // 4. إذا فما لغز حالي، اعرضه على الشاشة
        if (enigme_actuelle) {
            afficherEnigme(ecran, font, enigme_actuelle);
        }

        // 5. حدث الشاشة باش يظهر كل شيء رسمناه
        SDL_Flip(ecran);

        // تحكم في سرعة تحديث الشاشة (بالملي ثانية) - حاول تخليها قريبة لسرعة الكرونومتر باش تكون الحركة سلسة
        SDL_Delay(1000 / 60); // مثال: 60 إطار في الثانية
    }

    // تنظيف الذاكرة قبل ما نسكروا البرنامج
    libererAnimationChronometre(&chrono);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

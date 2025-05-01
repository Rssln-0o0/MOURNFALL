#include "enigme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// دالة تقوم بتحميل الألغاز من ملف نصي
int chargerEnigmes(const char *filename, GestionnaireEnigmes *gestionnaire) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening questions file"); // إذا ما تنجمش تفتح الملف، يطلع error
        return 0;
    }

    char line[QUESTION_LENGTH * 2]; // بافر باش نقراو بيه كل سطر من الملف
    gestionnaire->nombre_enigmes_chargees = 0;
    gestionnaire->dernier_index_pose = -1; // قيمة أولية باش ما يكونش عنده قيمة سؤال حقيقي

    // نقراو الأسطر من الملف لين نوصلوا للحد الأقصى للأسئلة ولا لين يوفى الملف
    while (fgets(line, sizeof(line), file) && gestionnaire->nombre_enigmes_chargees < MAX_QUESTIONS) {
        char *token = strtok(line, "|"); // نفصلوا السطر باستعمال العلامة "|"

        // الجزء الأول هو السؤال
        if (token) strcpy(gestionnaire->enigmes[gestionnaire->nombre_enigmes_chargees].question, token);

        // الأجزاء اللي بعده هما الأجوبة
        for (int i = 0; i < NOMBRE_CHOIX; i++) {
            token = strtok(NULL, "|");
            if (token) strcpy(gestionnaire->enigmes[gestionnaire->nombre_enigmes_chargees].answers[i], token);
        }

        // الجزء اللي بعد الأجوبة هو رقم الجواب الصحيح (نقصوا 1 باش يولي index)
        token = strtok(NULL, "|");
        gestionnaire->enigmes[gestionnaire->nombre_enigmes_chargees].correct_index = token ? atoi(token) - 1 : -1;

        // الجزء الأخير هو نوع الإنيغمة
        token = strtok(NULL, "|");
        if (token) strcpy(gestionnaire->enigmes[gestionnaire->nombre_enigmes_chargees].type, token);

        gestionnaire->nombre_enigmes_chargees++; // نزيدو في عدد الألغاز اللي قريناها
    }

    fclose(file); // نسكروا الملف بعد ما نكملوا قرايته
    return 1; // نرجع 1 يعني العملية تمت بنجاح
}

// دالة تقوم بعرض اللغز الحالي على الشاشة
void afficherEnigme(SDL_Surface *ecran, TTF_Font *font, const Enigme *enigme) {
    SDL_Color couleurBlanche = {255, 255, 255}; // لون أبيض للنص
    SDL_Surface *texte;
    SDL_Rect position = { .x = 50, .y = 50 }; // البلاصة اللي باش يبدا منها عرض السؤال

    texte = TTF_RenderText_Solid(font, enigme->question, couleurBlanche); // نرندروا (نرسموا) نص السؤال
    SDL_BlitSurface(texte, NULL, ecran, &position); // نحطوا النص المرسوم على الشاشة
    SDL_FreeSurface(texte); // نمسحوا السورفاس متاع النص باش ما نحجزش ذاكرة على الفاضي
    position.y += 50; // ننزلوا شوية باش نعرضوا الأجوبة تحت السؤال

    // نعرضوا كل جواب مرقم
    for (int i = 0; i < NOMBRE_CHOIX; i++) {
        char reponse[ANSWER_LENGTH + 10];
        snprintf(reponse, sizeof(reponse), "%d: %s", i + 1, enigme->answers[i]); // نركبوا الرقم مع النص
        texte = TTF_RenderText_Solid(font, reponse, couleurBlanche); // نرندروا الجواب
        SDL_BlitSurface(texte, NULL, ecran, &position); // نحطوه على الشاشة
        SDL_FreeSurface(texte); // نمسحوا السورفاس
        position.y += 30; // ننزلوا للسطر اللي بعده
    }

    SDL_Flip(ecran); // نحديثوا الشاشة باش يظهر كل شيء
}

// دالة تقوم بالتحقق من إجابة المستخدم
int verifierReponse(const Enigme *enigme, int reponse_utilisateur) {
    return reponse_utilisateur - 1 == enigme->correct_index; // نقارنوا إجابة المستخدم مع الفهرس الصحيح
}

// دالة تجلب لغز عشوائي من بين كل الألغاز المحملة (مع تجنب تكرار آخر لغز)
Enigme* obtenirEnigmeAleatoire(GestionnaireEnigmes *gestionnaire) {
    if (gestionnaire->nombre_enigmes_chargees == 0) {
        return NULL; // إذا ما فماش ألغاز محملة، نرجعوا NULL
    }

    int index_aleatoire;
    // نبقاو نختاروا في رقم عشوائي لين يكون مختلف عن آخر رقم تم اختياره (إذا فما أكثر من سؤال)
    do {
        index_aleatoire = rand() % gestionnaire->nombre_enigmes_chargees;
    } while (index_aleatoire == gestionnaire->dernier_index_pose && gestionnaire->nombre_enigmes_chargees > 1);

    gestionnaire->dernier_index_pose = index_aleatoire; // نسجلوا هذا الرقم باش ما نعاودوش نجيبوه في المرة الجاية
    return &gestionnaire->enigmes[index_aleatoire]; // نرجعوا مؤشر للغز اللي تم اختياره
}

// دالة تجلب لغز عشوائي من نوع معين (QCM أو Puzzle)
Enigme* obtenirEnigmeAleatoireByType(GestionnaireEnigmes *gestionnaire, const char *type) {
    if (gestionnaire->nombre_enigmes_chargees == 0) {
        return NULL; // إذا ما فماش ألغاز محملة، نرجعوا NULL
    }

    int nombre_disponibles = 0; // عدد الألغاز المتوفرة من النوع المطلوب
    int indices_disponibles[MAX_QUESTIONS]; // جدول باش نخزنوا فيه فهارس الألغاز المتوفرة
    // ندوروا على كل الألغاز ونشوفوا إذا كان نوعها هو النوع المطلوب
    for (int i = 0; i < gestionnaire->nombre_enigmes_chargees; i++) {
        if (strcmp(gestionnaire->enigmes[i].type, type) == 0) {
            indices_disponibles[nombre_disponibles++] = i; // نضيفوا الفهرس إذا كان النوع صحيح
        }
    }

    if (nombre_disponibles == 0) {
        return NULL; // إذا ما لقينا حتى لغز من النوع المطلوب، نرجعوا NULL
    }

    // نختاروا فهرس عشوائي من بين الفهارس المتاحة
    int index_aleatoire_disponible = rand() % nombre_disponibles;
    // نجيبوا الفهرس الحقيقي للغز من الجدول
    int index_enigme_reel = indices_disponibles[index_aleatoire_disponible];
    return &gestionnaire->enigmes[index_enigme_reel]; // نرجعوا مؤشر للغز اللي تم اختياره
}


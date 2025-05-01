#include "chronometre.h" // نستحقوا التعريفات اللي في chronometre.h
#include <stdio.h>   // باش نجموا نستعملوا حاجات كيف printf للأخطاء
#include <stdlib.h>  // باش نجموا نستعملوا حاجات كيف malloc باش نحجزوا ذاكرة
#include <string.h>  // باش نجموا نتعاملوا مع النصوص (أسماء الملفات)
#include <SDL/SDL_image.h> // نستحقوا SDL_image باش نحملوا التصاور بصيغ مختلفة كيف PNG

// دالة تقوم بتحميل تصاور الكرونومتر وتحطهم في الهيكل ChronometreAnimation
int chargerAnimationChronometre(const char *chemin_base, int nombre_images, Uint32 delai, ChronometreAnimation *chrono) {
    // نحجزوا بلاصة في الذاكرة باش نخزنوا فيها مؤشرات (pointers) لكل تصويرة
    chrono->frames = (SDL_Surface *)malloc(sizeof(SDL_Surface *) * nombre_images);
    // إذا ما تنجمناش نحجزوا ذاكرة، نطلعوا error ونرجعوا -1
    if (!chrono->frames) {
        fprintf(stderr, "Error allocating memory for chronometre frames\n");
        return -1;
    }
    chrono->nombre_frames = nombre_images; // نسجلوا قداش فما تصويرة
    chrono->frame_actuelle = 0;        // نبداو نعرضوا من أول تصويرة (index 0)
    chrono->delai_frame = delai;        // نسجلوا قداش لازم نستناو بين كل تصويرة
    chrono->temps_dernier_update = SDL_GetTicks(); // نسجلوا الوقت الحالي كآخر مرة بدلنا فيها التصويرة

    char nom_fichier[256]; // بافر باش نركبوا فيه اسم كل ملف تصويرة
    // ندوروا على كل تصويرة مرقمة (من 1 لين nombre_images)
    for (int i = 0; i < nombre_images; i++) {
        sprintf(nom_fichier, "%s%d.png", chemin_base, i + 1); // نركبوا اسم الملف (مثلاً chrono_1.png)
        chrono->frames[i] = IMG_Load(nom_fichier); // نحملوا التصويرة
        // إذا ما تنجمناش نحملوا التصويرة، نطلعوا error ونمسحوا أي تصاور حملناها قبل ونرجعوا -1
        if (!chrono->frames[i]) {
            fprintf(stderr, "Error loading image %s: %s\n", nom_fichier, SDL_GetError());
            for (int j = 0; j < i; j++) SDL_FreeSurface(chrono->frames[j]);
            free(chrono->frames);
            return -1;
        }
    }
    return 0; // كل شيء مشى بالباهي
}

// دالة تقوم بعرض التصويرة الحالية متاع الكرونومتر على الشاشة
void


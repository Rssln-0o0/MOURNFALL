#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H

#include <SDL/SDL.h> // نستحقوا الـ SDL باش نتعاملوا مع الرسوم والصور
#include <stdbool.h> // باش نجموا نستعملوا النوع البولياني (true و false)

// تعريف هيكل (structure) الكرونومتر اللي فيه الرسوم متاعو
typedef struct {
    SDL_Surface *frames;        // **frames:* كيما نقولوا "إطارات" فيلم الكرتون. هذا جدول كبير يخزن فيه كل تصويرة من تصاور حركة الكرونومتر. كل بلاصة في الجدول فيها تصويرة.
    int nombre_frames;         // *nombre_frames:* قداش عندنا من تصويرة (إطار) في الجدول هذا الكل. يعني قداش فما مرحلة في حركة الكرونومتر.
    int frame_actuelle;        // *frame_actuelle:* هذا متغير يقولنا توا إناهو تصويرة (إطار) قاعدين نعرضوا فيها بالضبط من الجدول. باش تبان الحركة، نبقاو نبدلوا في هالرقم.
    Uint32 delai_frame;        // *delai_frame:* هذا قداش نستناو بالملي ثانية بين ما نعرضوا تصويرة ونعرضوا التصويرة اللي بعدها. هو اللي يتحكم في سرعة حركة الكرونومتر. كل ما يكون الرقم صغير، تكون الحركة أسرع.
    Uint32 temps_dernier_update; // *temps_dernier_update:* هذا وقتاش آخر مرة بدلنا التصويرة. نستحقوه باش نعرفوا قداش فات وقت من وقتها ونقرروا إذا حان الوقت باش نبدلوا لتصويرة أخرى.
} ChronometreAnimation;

// تصريح بدالة (function) باش تحمل تصاور الكرونومتر وتحطهم في الهيكل ChronometreAnimation
// *const char *chemin_base:* وين موجودين التصاور متاع الكرونومتر (مثلاً "chrono_").
// *int nombre_images:* قداش فما تصويرة متاع الكرونومتر مرقمة (مثلاً من 1 لين 9).
// *Uint32 delai:* قداش نستناو بين كل تصويرة (بالملي ثانية).
// *ChronometreAnimation *chrono:* مؤشر (pointer) للهيكل متاع الكرونومتر اللي باش نخزنوا فيه التصاور والمعلومات.
int chargerAnimationChronometre(const char *chemin_base, int nombre_images, Uint32 delai, ChronometreAnimation *chrono);

// تصريح بدالة باش تعرض التصويرة الحالية متاع الكرونومتر على الشاشة
// *SDL_Surface *ecran:* الشاشة اللي باش نعرضوا عليها.
// *const ChronometreAnimation *chrono:* مؤشر للهيكل متاع الكرونومتر اللي فيه التصويرة اللي باش نعرضوها.
// *SDL_Rect position:* البلاصة في الشاشة وين باش نعرضوا الكرونومتر (الإحداثيات x و y).
void afficherAnimationChronometre(SDL_Surface *ecran, const ChronometreAnimation *chrono, SDL_Rect position);

// تصريح بدالة باش تبدل التصويرة المعروضة بالتصويرة اللي بعدها باش تبان الحركة
// *ChronometreAnimation *chrono:* مؤشر للهيكل متاع الكرونومتر اللي باش نحدثوا فيه الإطار الحالي.
void mettreAJourAnimationChronometre(ChronometreAnimation *chrono);

// تصريح بدالة باش نمسحوا التصاور اللي حملناهم من الذاكرة وقت اللي نسكروا اللعبة باش ما يصيرش مشكل
// *ChronometreAnimation *chrono:* مؤشر للهيكل متاع الكرونومتر اللي فيه التصاور اللي باش نمسحوها.
void libererAnimationChronometre(ChronometreAnimation *chrono);

// تصريح بدالة باش نرجعوا الكرونومتر يبدا من الأول (نعرضوا أول تصويرة)
// *ChronometreAnimation *chrono:* مؤشر للهيكل متاع الكرونومتر اللي باش نرجعوه للبداية.
void demarrerChronometre(ChronometreAnimation *chrono);

// تصريح بدالة (حالياً ما تعمل شيء مهم) باش نشوفوا إذا الكرونومتر وصل لنهاية عد معين ولا لا (ممكن نستعملوها في المستقبل)
// *const ChronometreAnimation *chrono:* مؤشر للهيكل متاع الكرونومتر.
bool isChronometreFini(const ChronometreAnimation *chrono);

#endif

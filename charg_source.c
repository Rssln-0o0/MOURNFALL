#include "head.h"
#include <SDL/SDL_mixer.h>

void initMENU(MENU *menuStruct_Charg)
{
          //ANIMATION PICS
  menuStruct_Charg->BACKGROUND=IMG_Load("img/save/bg1.png");
  menuStruct_Charg->ECRANJOUEUR=IMG_Load("img/save/joueur1.png");
  menuStruct_Charg->NOUV1=IMG_Load("img/save/nouvnon1.png");
  menuStruct_Charg->NOUV2=IMG_Load("img/save/nouvoui1.png");
  menuStruct_Charg->positionNOUV.x=150;
  menuStruct_Charg->positionNOUV.y=100;
  menuStruct_Charg->CHARGER1=IMG_Load("img/save/chargernon1.png");
  menuStruct_Charg->CHARGER2=IMG_Load("img/save/chargeroui1.png");
  menuStruct_Charg->positionCHARGER.x=150;
  menuStruct_Charg->positionCHARGER.y=250;
  menuStruct_Charg->OUI1=IMG_Load("img/save/ouinon1.png");
  menuStruct_Charg->OUI2=IMG_Load("img/save/ouioui1.png");
  menuStruct_Charg->positionOUI.x=260;
  menuStruct_Charg->positionOUI.y=250;
  menuStruct_Charg->NON1=IMG_Load("img/save/nonnon1.png");
  menuStruct_Charg->NON2=IMG_Load("img/save/nonoui1.png");
  menuStruct_Charg->positionNON.x=250;
  menuStruct_Charg->positionNON.y=350;
  (menuStruct_Charg->sonbutton)=Mix_LoadWAV("sounds/muse_char_select.wav");
  menuStruct_Charg->buttonon=0;
  menuStruct_Charg->GAMEPAGE=0;
  menuStruct_Charg->changemenu=1;
  menuStruct_Charg->police=TTF_OpenFont("text/blue.ttf",20);
  menuStruct_Charg->white.r=255;
  menuStruct_Charg->white.g=255;
  menuStruct_Charg->white.b=255;
  menuStruct_Charg->text=TTF_RenderText_Blended(menuStruct_Charg->police,"Do you want to save your game ?",menuStruct_Charg->white);
  menuStruct_Charg->positionTEXT.x=60;
  menuStruct_Charg->positionTEXT.y=150;
}

void MenuINPUTS(MENU *menuStruct_Charg,SDL_Event event)
{ 
  int x,y;
 while(SDL_PollEvent(&event))
    {   
  switch(event.type){
    

    case SDL_QUIT:
       SDL_Quit();
       break;

                       // CLICKS EL CLAVIER

    case SDL_KEYDOWN:
        switch(menuStruct_Charg->GAMEPAGE)
        {
        case 0:
             if(event.key.keysym.sym==SDLK_F5)
             {
                sonbref(menuStruct_Charg);
                menuStruct_Charg->GAMEPAGE++;
                menuStruct_Charg->changemenu=1;
             }
                                
            if(event.key.keysym.sym==SDLK_F9)
            {
              menuStruct_Charg->changemenu=1;

              sonbref(menuStruct_Charg);
              
            }
                  
break;
        case 1:
       
            if(event.key.keysym.sym==SDLK_n)
                { 
            menuStruct_Charg->GAMEPAGE++;
            menuStruct_Charg->changemenu=1;
            sonbref(menuStruct_Charg);
                }
            if(event.key.keysym.sym==SDLK_c)
                {
                    sonbref(menuStruct_Charg);
                    menuStruct_Charg->changemenu=1;

                }
            if(event.key.keysym.sym==SDLK_ESCAPE)
                {  
            exit(-1);
                }
            break;
        case 2 :
             if(event.key.keysym.sym==SDLK_ESCAPE)
                {  
            exit(-1);
                }
                break;

        }

                      //CLICK EL SOURIS

    case SDL_MOUSEBUTTONDOWN:
       if( event.button.button == SDL_BUTTON_LEFT )
        {
            switch(menuStruct_Charg->buttonon)
            {
            case 0:
              break;
            case 1:
              //menu li ba3d play;

                 sonbref(menuStruct_Charg);
              (menuStruct_Charg->GAMEPAGE)++;
              menuStruct_Charg->changemenu=1;
             
              break;
            case 2:
              
              //nbadel lel menu li ba3d Settings
              (menuStruct_Charg->GAMEPAGE)--;
              menuStruct_Charg->changemenu=1;
              sonbref(menuStruct_Charg);
              break;
            }
        }
        break;




     case SDL_MOUSEMOTION :
         x = event.motion.x;
         y = event.motion.y;
        
//RANGES L BUTTONS TA3 EL MENU ELOULA
         switch (menuStruct_Charg->GAMEPAGE) {
        case 0 :          
        //KEN EL SOURIS 3AL BUTTON
        if( ( x > menuStruct_Charg->positionOUI.x ) && ( x < menuStruct_Charg->positionOUI.x+100 ) && ( y > menuStruct_Charg->positionOUI.y ) && ( y < menuStruct_Charg->positionOUI.x+40  ) )
        {
          //variable animation l button chen7otou ON
          menuStruct_Charg->buttonon=1;
          // TCHOUF KEN DEJA L BUTTON SELECTIONEE FIL FRAME LI 9ABLOU , SNN TWALI LEZEM TREFRESHI L AFFICHAGE MBA3ED
          if (menuStruct_Charg->SELECTEDBUT!=1){
              sonbref(menuStruct_Charg);
              menuStruct_Charg->changemenu=1;

            menuStruct_Charg->SELECTEDBUT=1;
          }
            
         }
           else
          { 
               if( ( x >= menuStruct_Charg->positionNON.x ) && ( x <= menuStruct_Charg->positionNON.x+100) && ( y >= (menuStruct_Charg->positionNON.y) ) && ( y <=  menuStruct_Charg->positionNON.y+40 ) )
              //variable animation l button chen7otou ON
               { 
                menuStruct_Charg->buttonon=2;
                if (menuStruct_Charg->SELECTEDBUT!=2)
                {
                     sonbref(menuStruct_Charg);
               
                menuStruct_Charg->changemenu=1;
                menuStruct_Charg->SELECTEDBUT=2;
                 }
               }
               else
                menuStruct_Charg->SELECTEDBUT=0;
           
          }
          break;


//EL RANGES TA3 L BUTTONS TA3 L MENU LI BA3D BUTTON PLAY
        case 1 :
        if( ( x > menuStruct_Charg->positionNOUV.x ) && ( x < menuStruct_Charg->positionNOUV.x+400) && ( y > menuStruct_Charg->positionNOUV.y ) && ( y < menuStruct_Charg->positionNOUV.y+40 ) )
        {
            

           menuStruct_Charg->buttonon=1;
          if (menuStruct_Charg->SELECTEDBUT!=1){
              sonbref(menuStruct_Charg);
              menuStruct_Charg->changemenu=1;

            menuStruct_Charg->SELECTEDBUT=1;
          }
        }
        else
        {
          if( ( x > menuStruct_Charg->positionCHARGER.x ) && ( x < menuStruct_Charg->positionCHARGER.x+400) && ( y > menuStruct_Charg->positionCHARGER.y ) && ( y < menuStruct_Charg->positionCHARGER.y+40 ) )
           {
              menuStruct_Charg->buttonon=1;
             if (menuStruct_Charg->SELECTEDBUT!=2)
                {
                     sonbref(menuStruct_Charg);
               
                menuStruct_Charg->changemenu=1;
                menuStruct_Charg->SELECTEDBUT=2;
                 }
           }
           else
            menuStruct_Charg->SELECTEDBUT=0;
        menuStruct_Charg->changemenu=1;
    }
          break;
//EL RANGES TA3 L BUTTONS TA3 L MENU LI BA3D BUTTON NEWGAME
          
           
}
 }
 } 
  }


 void afficherMENU(MENU *menuStruct_Charg,SDL_Surface *screen)
 {  
   switch(menuStruct_Charg->GAMEPAGE)
   {
   case 0 :
   sonbref(menuStruct_Charg);
    // TCHOUF ENEHI LI SELECTED SNN T AFFICHI EL BUTTON EL NORMAL

    if (menuStruct_Charg->SELECTEDBUT==1)
    
            SDL_BlitSurface(menuStruct_Charg->OUI2,NULL,screen,&(menuStruct_Charg->positionOUI));
    else
            SDL_BlitSurface(menuStruct_Charg->OUI1,NULL,screen,&(menuStruct_Charg->positionOUI));
    if (menuStruct_Charg->SELECTEDBUT==2)
            SDL_BlitSurface(menuStruct_Charg->NON2,NULL,screen,&(menuStruct_Charg->positionNON));
    else
            SDL_BlitSurface(menuStruct_Charg->NON1,NULL,screen,&(menuStruct_Charg->positionNON));
   
       menuStruct_Charg->changemenu=0;
    //     menu->SELECTEDBUT=0;
       
  //   }
    break;

   case 1:
    if (menuStruct_Charg->changemenu==1){
    SDL_BlitSurface(menuStruct_Charg->BACKGROUND,NULL,screen,NULL);
   
    // TCHOUF ENEHI LI SELECTED SNN T AFFICHI EL BUTTON EL NORMAL

    if (menuStruct_Charg->SELECTEDBUT==1)
    
            SDL_BlitSurface(menuStruct_Charg->NOUV2,NULL,screen,&(menuStruct_Charg->positionNOUV));
    else
            SDL_BlitSurface(menuStruct_Charg->NOUV1,NULL,screen,&(menuStruct_Charg->positionNOUV));
    if (menuStruct_Charg->SELECTEDBUT==2)
            SDL_BlitSurface(menuStruct_Charg->CHARGER2,NULL,screen,&(menuStruct_Charg->positionCHARGER));
    else
            SDL_BlitSurface(menuStruct_Charg->CHARGER1,NULL,screen,&(menuStruct_Charg->positionCHARGER));
   
       menuStruct_Charg->changemenu=0;
     //    menu->SELECTEDBUT=0;
       
     }
      break;
   case 2:
      SDL_BlitSurface(menuStruct_Charg->ECRANJOUEUR,NULL,screen,NULL);
    break;
    } 
}

   
void sonbref(MENU *menuStruct_Charg)
{
  Mix_PlayChannel(-1, menuStruct_Charg->sonbutton, 0);
}


void displaySaveMenu(SDL_Surface* screen, TTF_Font* font, Mix_Music* bgMusic) {
  MENU menu;
  SDL_Event event;
  int running = 1;

  // Initialize the save menu
  initMENU(&menu);
  Mix_PlayMusic(menu.music, -1);

  while (running) {
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
              running = 0;
          }

          // Handle inputs for the save menu
          MenuINPUTS(&menu, event);

          // Check if the user wants to exit the save menu
          if (menu.GAMEPAGE == 2) { // Example condition to exit
              running = 0;
          }
      }

      // Render the save menu
      afficherMENU(&menu, screen);
      SDL_Flip(screen);
      SDL_Delay(16);
  }

  // Clean up resources
  SDL_FreeSurface(menu.BACKGROUND);
  SDL_FreeSurface(menu.ECRANJOUEUR);
  SDL_FreeSurface(menu.NOUV1);
  SDL_FreeSurface(menu.NOUV2);
  SDL_FreeSurface(menu.CHARGER1);
  SDL_FreeSurface(menu.CHARGER2);
  SDL_FreeSurface(menu.OUI1);
  SDL_FreeSurface(menu.OUI2);
  SDL_FreeSurface(menu.NON1);
  SDL_FreeSurface(menu.NON2);
  Mix_FreeMusic(menu.music);
  Mix_FreeChunk(menu.sonbutton);
  TTF_CloseFont(menu.police);
}
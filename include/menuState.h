#ifndef MENU_STATE_H
#define MENU_STATE_H
#include "SDL2/SDL.h"
#include "stdbool.h"
#include "SDL2/SDL_ttf.h"
#include "utils.h"

typedef struct MenuState
{
    bool quit;
    TTF_Font *font;
    IntInRange selectedOption;
} MenuState;

void MenuState_init(MenuState *menuState, SDL_Renderer *renderer, TTF_Font *font);
void MenuState_free(MenuState *menuState);
void MenuState_event(MenuState *menuState, SDL_Event event);
void MenuState_update(MenuState *menuState, float delta);
void MenuState_draw(MenuState *menuState, SDL_Renderer *renderer);

#endif
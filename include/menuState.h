#ifndef MENU_STATE_H
#define MENU_STATE_H
#include "SDL2/SDL.h"
#include "stdbool.h"
#include "SDL2/SDL_ttf.h"
#include "utils.h"

typedef struct menuState_t
{
    bool quit;
    TTF_Font *font;
    intInRange_t selectedOption;
} menuState_t;

void menuState_init(menuState_t *menuState, SDL_Renderer *renderer, TTF_Font *font);
void menuState_free(menuState_t *menuState);
void menuState_event(menuState_t *menuState, SDL_Event event);
void menuState_update(menuState_t *menuState, float delta);
void menuState_draw(menuState_t *menuState, SDL_Renderer *renderer);

#endif
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "global.h"
#include "draw.h"
#include "drawRaycast.h"
#include "SDL2/SDL.h"
#include "collision.h"
#include "projectile.h"
#include "enemy.h"

#include "SDL2/SDL_ttf.h"

typedef struct gameState_t
{
    ray_t *rays;
    player_t player;
    map_t map;
    enemy_arr_t enemies;
    projectile_arr_t enemy_projectiles;
    keys_t keys;
    float fZBuffer[SCREEN_WIDTH];
    SDL_Surface *screenSurface;
    SDL_Surface *deathScreenSurface;
    SDL_Texture *bar_texture;
    SDL_Texture *frame_texture;
    SDL_Texture *gun_texture;
    SDL_Texture *soldier_texture;
    SDL_Texture *walls_texture;
    SDL_Texture *vignete_texture;
    SDL_Surface *walls_surface;
    SDL_Surface *soldier_surface;
    text_texture_t youDiedText;
    uint32_t rndval;
    bool deathAnimFinished;
    bool quit;
    TTF_Font *font;
    bool paused;
    intInRange_t selectedOptionPaused;
} gameState_t;

// typedef gameStatePausedMenu_t
// {
//     int optionsCount;
//     int selectedOption;
//     const char *options[2];
// }
// gameStatePausedMenu_t;

void gameState_init(gameState_t *gameState, SDL_Renderer *renderer, TTF_Font *font);
void gameState_free(gameState_t *gameState);
void gameState_event(gameState_t *gameState, SDL_Event event);
void gameState_update(gameState_t *gameState, float delta);
void update_enemies(enemy_arr_t *enemies, projectile_arr_t *enemy_projectiles, player_t *player, float delta);
void player_shoot_enemies_raycast(enemy_arr_t *enemies, player_t *player, map_t *map, float maxDistance);
void update_projectiles(projectile_arr_t *projectiles, float delta);
void gameState_draw(gameState_t *gameState, SDL_Renderer *renderer);
void player_attack_enemies(enemy_arr_t *enemies, player_t *player, map_t *map);

#endif

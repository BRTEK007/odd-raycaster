#ifndef DRAW_H
#define DRAW_H

#define SQUARE_ROOT_OF_TWO 1.4142

#include "SDL2/SDL.h"
#include "player.h"
#include "rays.h"
#include "enemy.h"
#include "global.h"
#include "utils.h"
#include "sprite.h"
#include "projectile.h"
#include "map.h"
#include "SDL2/SDL_ttf.h"

typedef struct draw_floor_data_t
{
    SDL_Surface *screenSurface;
    player_t *player;
    SDL_Surface *walls_surface;
    int startY;
    int endY;
} draw_floor_data_t;

typedef struct draw_walls_data_t
{
    SDL_Surface *screenSurface;
    player_t *player;
    ray_t *rays;
    float *zBuffer;
    SDL_Surface *walls_surface;
    int startX, endX;
} draw_walls_data_t;

typedef struct scaling_info_t
{
    unsigned scalingFactor;
    unsigned offsetX;
    unsigned offsetY;
    unsigned rendererWidth;
    unsigned rendererHeight;
} scaling_info_t;

typedef struct text_texture_t
{
    SDL_Texture *texture;
    float aspectRatio;
} text_texture_t;

void text_texture_free(text_texture_t *tt);
text_texture_t create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color);

scaling_info_t get_scaling_info(SDL_Renderer *renderer);
void *draw_floor_threaded(void *vargp);
void *draw_walls_threaded(void *vargp);
void draw_text(SDL_Renderer *renderer, SDL_Texture *texture, int sx, int sy, char *text);
void draw_map(SDL_Renderer *renderer, player_t *player, enemy_arr_t *enemies, ray_t *rays, int iRayCount, map_t *map);
void draw_weapon(SDL_Renderer *renderer, player_t *player, SDL_Texture *gun_texture, scaling_info_t *scalingInfo);
void draw_bar(SDL_Renderer *renderer, player_t *player, SDL_Texture *bar_texture, scaling_info_t *scalingInfo);
void draw_paused_bar(SDL_Renderer *renderer, TTF_Font *font, scaling_info_t *scalingInfo, int selectedOption);

#endif

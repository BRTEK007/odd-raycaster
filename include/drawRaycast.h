#ifndef DRAW_RAYCAST_H
#define DRAW_RAYCAST_H

#include "draw.h"

void draw_floor_and_ceiling(SDL_Surface *screenSurface, player_t *player, SDL_Surface *walls_surface, int startY, int endY);
void draw_walls(SDL_Surface *screenSurface, player_t *player, ray_t *rays, float *fZBuffer, SDL_Surface *walls_surface, int startX, int endX);
void draw_sprites(SDL_Renderer *renderer, player_t *player, sprite_arr_t *sprites, float *fZBuffer, SDL_Texture *soldier_texture, scaling_info_t *scalingInfo);

int sprite_dist_compare(const void *a, const void *b);

#endif
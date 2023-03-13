#ifndef DRAW_RAYCAST_H
#define DRAW_RAYCAST_H

#include "draw.h"

void drawFloorNCeiling(SDL_Surface *screenSurface, Player *player, SDL_Surface *walls_surface, int startY, int endY);
void drawWalls(SDL_Surface *screenSurface, Player *player, Ray *rays, float *fZBuffer, SDL_Surface *walls_surface, int startX, int endX);
void drawSprites(SDL_Renderer *renderer, Player *player, SpriteArray *sprites, float *fZBuffer, SDL_Texture *soldier_texture, ScalingData *scalingInfo);

int Sprite_compareDistance(const void *a, const void *b);

#endif
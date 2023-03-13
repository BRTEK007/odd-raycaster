#ifndef RAYS_H
#define RAYS_H

#include "vec2f.h"
#include "player.h"
#include "global.h"
#include "map.h"

typedef enum BLOCK_SIDE
{
    VERTICAL,
    HORIZONTAL
} BLOCK_SIDE;

typedef struct ray_t
{
    vec2f vHit;
    vec2f vRayDir;
    bool bHit;
    float fDistance;
    float fCorrectedDistance;
    BLOCK_SIDE blockSide;
    bool diagonal;
    int iBlockId;
    int iTextureId;
} ray_t;

int raySegCollision(vec2f rayStart, vec2f rayDir, vec2f segA, vec2f segB, vec2f *inter);
void cast_rays(player_t *player, ray_t *rays, map_t *map);
float rayPointDistSquared(vec2f rayStart, vec2f rayDir, vec2f point);
void getSegmentFromDiagonalWall(char blockType, vec2f *segA, vec2f *segB);
bool didRayHitDiagonalWall(ray_t ray, char blockType);
ray_t castSinleRay(vec2f rayStart, vec2f rayDir, map_t *map);

#endif // RAYS_H

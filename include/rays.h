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

typedef struct Ray
{
    Vector2f hitPos;
    Vector2f dir;
    bool isHit;
    float distance;
    float correctedDistance;
    BLOCK_SIDE blockSide;
    bool isDiagonal;
    int blockId;
    int textureId;
} Ray;

int raySegCollision(Vector2f rayStart, Vector2f rayDir, Vector2f segA, Vector2f segB, Vector2f *inter);
void castRays(Player *player, Ray *rays, Map *map);
float rayPointDistSquared(Vector2f rayStart, Vector2f rayDir, Vector2f point);
void getSegmentFromDiagonalWall(char blockType, Vector2f *segA, Vector2f *segB);
bool didRayHitDiagonalWall(Ray ray, char blockType);
Ray castOneRay(Vector2f rayStart, Vector2f rayDir, Map *map);

#endif // RAYS_H

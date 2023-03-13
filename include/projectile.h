#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdlib.h>
#include "vec2f.h"
#include "cute_c2.h"
#include "sprite.h"

typedef struct Projectile
{
    Vector2f pos;
    c2Circle circle;
    Vector2f dir;
    float lifetime;
} Projectile;

typedef struct ProjectileArray
{
    int size;
    int capacity;
    Projectile *arr;
} ProjectileArray;

void ProjectileArray_init(ProjectileArray *arr, int s);

void ProjectileArray_free(ProjectileArray *arr);

void ProjectileArray_add(ProjectileArray *arr, Projectile p);

void ProjectileArray_remove(ProjectileArray *arr, int index);

Sprite Projectile_getSprite(Projectile *e);

#endif
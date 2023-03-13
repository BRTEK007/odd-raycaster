#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>
#include "cute_c2.h"
#include "vec2f.h"

typedef struct Sprite
{
    Vector2f pos;
    float distanceToPlayerSquared;
    unsigned tx;
    unsigned ty;
    unsigned tw;
    unsigned th;
} Sprite;

typedef struct SpriteArray
{
    int size;
    int capacity;
    Sprite *arr;
} SpriteArray;

void SpriteArray_init(SpriteArray *a, int s);

void SpriteArray_free(SpriteArray *a);

void SpriteArray_add(SpriteArray *a, Sprite s);

#endif
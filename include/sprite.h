#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>
#include "cute_c2.h"
#include "vec2f.h"

typedef struct sprite_t
{
    vec2f pos;
    float fDistanceToPlayerSquared;
    unsigned tx;
    unsigned ty;
    unsigned tw;
    unsigned th;
} sprite_t;

typedef struct sprite_arr_t
{
    int size;
    int capacity;
    sprite_t *arr;
} sprite_arr_t;

void sprite_arr_init(sprite_arr_t *a, int s);

void sprite_arr_free(sprite_arr_t *a);

void sprite_arr_add(sprite_arr_t *a, sprite_t s);

#endif
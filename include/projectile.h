#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdlib.h>
#include "vec2f.h"
#include "cute_c2.h"
#include "sprite.h"

typedef struct projectile_t
{
    vec2f pos;
    c2Circle circle;
    vec2f vDir;
    float fLifetime;
} projectile_t;

typedef struct projectile_arr_t
{
    int size;
    int capacity;
    projectile_t *arr;
} projectile_arr_t;

void init_procetile_arr(projectile_arr_t *arr, int s);

void free_procetile_arr(projectile_arr_t *arr);

void add_procetile(projectile_arr_t *arr, projectile_t p);

void projectile_arr_remove(projectile_arr_t *arr, int index);

sprite_t projectile_get_sprite(projectile_t *e);

#endif
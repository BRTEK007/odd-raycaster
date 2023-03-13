#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_WIDTH 0.75          // 24/32
#define ENEMY_HIT_ANIM_TIME 0.25f // how long hit anim plays
#define ENEMY_SHOOT_DELAY 1.25F   // delay between shots

#include <stdlib.h>
#include "cute_c2.h"
#include "vec2f.h"
#include "sprite.h"
#include <stdbool.h>

typedef struct enemy_t
{
    vec2f pos;
    c2Circle circle;
    float fAttackCounter;
    float fHitAnimCounter;
    bool hitAnimPlaying;
    float width;
} enemy_t;

typedef struct enemy_arr_t
{
    int size;
    int capacity;
    enemy_t *enemies;
} enemy_arr_t;

void init_enemy_arr(enemy_arr_t *ea, int s);

void free_enemy_arr(enemy_arr_t *ea);

void add_enemy(enemy_arr_t *ea, float x, float y);

void enemy_take_damage(enemy_t *e);

sprite_t enemy_get_sprite(enemy_t *e);

#endif
#include "enemy.h"

void init_enemy_arr(enemy_arr_t *ea, int s)
{
    ea->size = 0;
    ea->capacity = s;
    ea->enemies = calloc(s, sizeof(enemy_t));
    if (ea->enemies == NULL)
        exit(1);
}

void free_enemy_arr(enemy_arr_t *ea)
{
    free(ea->enemies);
}

void add_enemy(enemy_arr_t *ea, float x, float y)
{
    if (ea->size == ea->capacity)
        return;
    enemy_t e;
    e.pos.x = x;
    e.pos.y = y;
    e.circle.p.x = 4.5f;
    e.circle.p.y = 4.5f;
    e.circle.r = ENEMY_WIDTH * 0.5f;
    e.width = ENEMY_WIDTH;
    e.fAttackCounter = 0.0f;
    e.fHitAnimCounter = 0.0f;
    e.hitAnimPlaying = false;
    ea->enemies[ea->size] = e;
    ea->size++;
}

sprite_t enemy_get_sprite(enemy_t *e)
{
    sprite_t s;
    s.pos = e->pos;
    s.tx = e->hitAnimPlaying ? 32 + 1 : 0;
    s.ty = 0;
    s.tw = 32;
    s.th = 32;
    return s;
}

void enemy_take_damage(enemy_t *e)
{
    e->fAttackCounter = 0.0f;
    e->fHitAnimCounter = 0.0f;
    e->hitAnimPlaying = true;
}
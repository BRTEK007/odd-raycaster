#include "enemy.h"

void EnemyArray_init(EnemyArray *ea, int s)
{
    ea->size = 0;
    ea->capacity = s;
    ea->enemies = calloc(s, sizeof(Enemy));
    if (ea->enemies == NULL)
        exit(1);
}

void EnemyArray_free(EnemyArray *ea)
{
    free(ea->enemies);
}

void EnemyArray_add(EnemyArray *ea, float x, float y)
{
    if (ea->size == ea->capacity)
        return;
    Enemy e;
    e.pos.x = x;
    e.pos.y = y;
    e.circle.p.x = 4.5f;
    e.circle.p.y = 4.5f;
    e.circle.r = ENEMY_WIDTH * 0.5f;
    e.width = ENEMY_WIDTH;
    e.attackTimeCounter = 0.0f;
    e.hitAnimTimeCounter = 0.0f;
    e.isHitAnimPlaying = false;
    ea->enemies[ea->size] = e;
    ea->size++;
}

Sprite Enemy_getSprite(Enemy *e)
{
    Sprite s;
    s.pos = e->pos;
    s.tx = e->isHitAnimPlaying ? 32 + 1 : 0;
    s.ty = 0;
    s.tw = 32;
    s.th = 32;
    return s;
}

void Enemy_takeDamage(Enemy *e)
{
    e->attackTimeCounter = 0.0f;
    e->hitAnimTimeCounter = 0.0f;
    e->isHitAnimPlaying = true;
}
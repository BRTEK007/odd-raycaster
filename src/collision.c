#include "collision.h"

void setPolyVerts(c2Poly *poly, char blockType)
{
    if (blockType == 'A')
    {
        poly->verts[0] = (c2v){.x = 0, .y = 1};
        poly->verts[1] = (c2v){.x = 1, .y = 0};
        poly->verts[2] = (c2v){.x = 1, .y = 1};
    }
    else if (blockType == 'B')
    {
        poly->verts[0] = (c2v){.x = 0, .y = 0};
        poly->verts[1] = (c2v){.x = 1, .y = 1};
        poly->verts[2] = (c2v){.x = 0, .y = 1};
    }
    else if (blockType == 'C')
    {
        poly->verts[0] = (c2v){.x = 0, .y = 0};
        poly->verts[1] = (c2v){.x = 1, .y = 0};
        poly->verts[2] = (c2v){.x = 1, .y = 1};
    }
    else if (blockType == 'D')
    {
        poly->verts[0] = (c2v){.x = 0, .y = 0};
        poly->verts[1] = (c2v){.x = 1, .y = 0};
        poly->verts[2] = (c2v){.x = 0, .y = 1};
    }
}

void player_map_collision(player_t *player, map_t *map)
{
    circleEntityMapCollision(&player->pos, player->circle, map);
}

void enemies_map_collision(enemy_arr_t *enemies, map_t *map)
{

    for (int i = 0; i < enemies->size; i++) // ENEMY WALL
    {
        enemy_t *e = &enemies->enemies[i];
        circleEntityMapCollision(&e->pos, e->circle, map);
    }
}

void projectiles_map_collision(projectile_arr_t *projectiles, map_t *map)
{
    for (int i = 0; i < projectiles->size; i++) // ENEMY WALL
    {
        projectile_t *p = &(projectiles->arr[i]);
        if (circleProjectileMapCollision(p->circle, map))
        {
            projectile_arr_remove(projectiles, i);
            i--;
        }
    }
}

bool circleProjectileMapCollision(c2Circle circle, map_t *map)
{
    // printf("%d %d %d\n", circle.p.x, circle.p.y, circle.r);
    int minX = (int)circle.p.x - 1;
    if (minX < 0)
        minX = 0;

    int maxX = (int)circle.p.x + 1;
    if (maxX >= map->width)
        maxX = map->width - 1;

    int minY = (int)circle.p.y - 1;
    if (minY < 0)
        minY = 0;

    int maxY = (int)circle.p.y + 1;
    if (maxY >= map->height)
        maxY = map->height - 1;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            if (map->arr[y][x].blockType == '#')
                continue;

            int result;

            if (map->arr[y][x].blockType == 'E')
            {
                c2AABB aabb;
                aabb.min.x = x;
                aabb.min.y = y;
                aabb.max.x = (float)x + 1.0f;
                aabb.max.y = (float)y + 1.0f;
                result = c2CircletoAABB(circle, aabb);
            }
            else
            {
                c2Poly poly;
                poly.count = 3;
                c2x polyTrans;
                polyTrans.p = (c2v){.x = x, .y = y};
                polyTrans.r = (c2r){.c = 1, .s = 0};
                setPolyVerts(&poly, map->arr[y][x].blockType);
                c2MakePoly(&poly);
                result = c2CircletoPoly(circle, &poly, &polyTrans);
            }

            if (result != 0) // resolve collision
                return true;
        }
    }
    return false;
}

void circleEntityMapCollision(vec2f *entityPos, c2Circle entityCircle, map_t *map)
{
    int minX = (int)entityPos->x - 1;
    if (minX < 0)
        minX = 0;

    int maxX = (int)entityPos->x + 1;
    if (maxX >= map->width)
        maxX = map->width - 1;

    int minY = (int)entityPos->y - 1;
    if (minY < 0)
        minY = 0;

    int maxY = (int)entityPos->y + 1;
    if (maxY >= map->height)
        maxY = map->height - 1;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            if (map->arr[y][x].blockType == '#')
                continue;

            c2Manifold m;
            if (map->arr[y][x].blockType == 'E')
            {
                c2AABB aabb;
                aabb.min.x = x;
                aabb.min.y = y;
                aabb.max.x = (float)x + 1.0f;
                aabb.max.y = (float)y + 1.0f;
                c2CircletoAABBManifold(entityCircle, aabb, &m);
            }
            else
            {
                c2Poly poly;
                poly.count = 3;
                c2x polyTrans;
                polyTrans.p = (c2v){.x = x, .y = y};
                polyTrans.r = (c2r){.c = 1, .s = 0};
                setPolyVerts(&poly, map->arr[y][x].blockType);
                c2MakePoly(&poly);
                c2CircletoPolyManifold(entityCircle, &poly, &polyTrans, &m);
            }

            if (m.count > 0) // resolve collision
            {
                vec2f vContact;
                vContact.x = m.contact_points[0].x;
                vContact.y = m.contact_points[0].y;

                vec2f vToPlayer = vector_sub(*entityPos, vContact);
                vToPlayer = vector_norm(vToPlayer);
                vToPlayer = vector_mult_scalar(vToPlayer, m.depths[0]);

                *entityPos = vector_add(*entityPos, vToPlayer);
            } // resolve collision
        }
    }
}

void player_enemy_projectiles_collision(player_t *player, projectile_arr_t *projectiles)
{
    for (int i = 0; i < projectiles->size; i++)
    {
        projectile_t *p = &(projectiles->arr[i]);
        if (c2CircletoCircle(p->circle, player->circle))
        {
            projectile_arr_remove(projectiles, i);
            i--;
            player_take_hit(player);
            if (player->isDead)
                return;
        }
    }
}
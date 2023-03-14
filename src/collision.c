/** 
    @file collision.c
    @brief collision resolution and detection
 */
#include "collision.h"


/**
 * @fn void setPolyVerts(c2Poly *poly, char blockType)
 * @brief Resolves collision between player and a map.
 * @param poly polygon which verts will be set
 * @param blockType type of block
 */
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


/**
 * @fn void resCollPlayerXMap(Player *player, Map *map)
 * @brief Resolves collision between player and a map.
 * @param player player
 * @param map map
 */
void resCollPlayerXMap(Player *player, Map *map)
{
    resCollCircleEntityXMap(&player->pos, player->circle, map);
}

/**
 * @fn void resCollEnemyArrayXMap(EnemyArray *enemies, Map *map)
 * @brief Resolves collision between enemies and a map.
 * @param enemies array of enemies
 * @param map map
 */
void resCollEnemyArrayXMap(EnemyArray *enemies, Map *map)
{

    for (int i = 0; i < enemies->size; i++) // ENEMY WALL
    {
        Enemy *e = &enemies->enemies[i];
        resCollCircleEntityXMap(&e->pos, e->circle, map);
    }
}

/**
 * @fn void resCollProjectileArrayXMap(ProjectileArray *projectiles, Map *map)
 * @brief Resolves collision between projectiles and a map. (projectiles get removed)
 * @param projectiles array of projectiles
 * @param map map
 */
void resCollProjectileArrayXMap(ProjectileArray *projectiles, Map *map)
{
    for (int i = 0; i < projectiles->size; i++) // ENEMY WALL
    {
        Projectile *p = &(projectiles->arr[i]);
        if (isCollCircleEntityXMap(p->circle, map))
        {
            ProjectileArray_remove(projectiles, i);
            i--;
        }
    }
}

/**
 * @fn bool isCollCircleEntityXMap(c2Circle circle, Map *map)
 * @brief Checks for collision between circle entity and a map.
 * @param circle circle collider of an entity
 * @param map map
 * @return true if collision found, false otherwise
 */
bool isCollCircleEntityXMap(c2Circle circle, Map *map)
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

/**
 * @fn void resCollCircleEntityXMap(Vector2f *entityPos, c2Circle entityCircle, Map *map)
 * @brief Resolves collision between circle entity and a map.
 * @param entityPos pointer to entity position, might be changed
 * @param entityCircle entity collider
 * @param map map
 */
void resCollCircleEntityXMap(Vector2f *entityPos, c2Circle entityCircle, Map *map)
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
                Vector2f vContact;
                vContact.x = m.contact_points[0].x;
                vContact.y = m.contact_points[0].y;

                Vector2f vToPlayer = Vector2f_sub(*entityPos, vContact);
                vToPlayer = Vector2f_norm(vToPlayer);
                vToPlayer = Vector2f_multScalar(vToPlayer, m.depths[0]);

                *entityPos = Vector2f_add(*entityPos, vToPlayer);
            } // resolve collision
        }
    }
}

/**
 * @fn void resCollPlayerXProjectileArray(Player *player, ProjectileArray *projectiles)
 * @brief Resolves collision between projectiles and a player.
 * @param player player
 * @param projectiles projectiles
 */
void resCollPlayerXProjectileArray(Player *player, ProjectileArray *projectiles)
{
    for (int i = 0; i < projectiles->size; i++)
    {
        Projectile *p = &(projectiles->arr[i]);
        if (c2CircletoCircle(p->circle, player->circle))
        {
            ProjectileArray_remove(projectiles, i);
            i--;
            Player_takeHit(player);
            if (player->isDead)
                return;
        }
    }
}
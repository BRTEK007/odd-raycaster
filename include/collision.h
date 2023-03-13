#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "map.h"
#include "projectile.h"
#include "enemy.h"
#include "cute_c2.h"
#include <stdbool.h>

void player_enemy_projectiles_collision(player_t *player, projectile_arr_t *projectiles);
void player_map_collision(player_t *player, map_t *map);
void enemies_map_collision(enemy_arr_t *enemies, map_t *map);
void projectiles_map_collision(projectile_arr_t *projectiles, map_t *map);
void setPolyVerts(c2Poly *poly, char blockType);
void circleEntityMapCollision(vec2f *entityPos, c2Circle entityCircle, map_t *map);
bool circleProjectileMapCollision(c2Circle entityCircle, map_t *map);

#endif
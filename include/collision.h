#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "map.h"
#include "projectile.h"
#include "enemy.h"
#include "cute_c2.h"
#include <stdbool.h>

void resCollPlayerXProjectileArray(Player *player, ProjectileArray *projectiles);
void resCollPlayerXMap(Player *player, Map *map);
void resCollEnemyArrayXMap(EnemyArray *enemies, Map *map);
void resCollProjectileArrayXMap(ProjectileArray *projectiles, Map *map);
void setPolyVerts(c2Poly *poly, char blockType);
void resCollCircleEntityXMap(Vector2f *entityPos, c2Circle entityCircle, Map *map);
bool isCollCircleEntityXMap(c2Circle entityCircle, Map *map);

#endif
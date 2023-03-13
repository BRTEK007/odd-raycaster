#ifndef PLAYER_H
#define PLAYER_H

#include "vec2f.h"
#include "input.h"
#include "cute_c2.h"
#include "utils.h"

#define GUN_ANIM_DELAY 0.075f
#define HEAD_BOB_SPEED 3.6f
#define HEAD_BOB_AMPLITUDE 7.0f
#define PLAYER_MAX_HEALTH 6
#define WEAPON_HANDGUN 0
#define WEAPON_LAUNCHER 1
#define WEAPON_KNIFE 2
#define KNIFE_REACH 1

typedef struct player_t
{
    vec2f pos;
    vec2f dir;
    vec2f plane;
    int iGunFrame;
    bool bIsShooting;
    float fGunAnimTimeCounter;
    c2Circle circle;
    float fOffsetVertical;
    float fVerticalAngle;
    bool bJustAttacked;
    int health;
    bool isDead;
    float fHitAnimOpacity;
    int selectedWeapon;
} player_t;

void update_player(player_t *player, keys_t *keys, float delta);

void init_player(player_t *player);

void player_take_hit(player_t *player);

#endif // PLAYER_H

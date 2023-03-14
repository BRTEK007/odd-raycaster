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

typedef struct Player
{
    Vector2f pos;
    Vector2f dir;
    Vector2f plane;
    int gunFrame;
    bool isShooting;
    float gunAnimTimeCounter;
    c2Circle circle;
    float offsetVertical;
    float verticalAngle;
    bool isJustAttacked;
    int health;
    bool isDead;
    float hitAnimOpacity;
    int selectedWeapon;
} Player;

void Player_update(Player *player, KeysArray *keys, float delta);

void Player_init(Player *player);

void Player_takeHit(Player *player);

#endif // PLAYER_H

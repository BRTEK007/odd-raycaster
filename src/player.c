#include "player.h"

void update_player(player_t *player, keys_t *keys, float delta)
{
    int iRotationMovement = (int)get_key(keys, SDLK_RIGHT).isPressed - (int)get_key(keys, SDLK_LEFT).isPressed;
    if (iRotationMovement != 0)
    {
        float a = PI * 0.5f * delta * iRotationMovement;
        float cosA = cos(a);
        float sinA = sin(a);
        vec2f v = player->dir;
        player->dir.x = v.x * cosA + v.y * (-sinA);
        player->dir.y = v.x * sinA + v.y * cosA;

        vec2f v2 = player->plane;
        player->plane.x = v2.x * cosA + v2.y * (-sinA);
        player->plane.y = v2.x * sinA + v2.y * cosA;
    }

    int iForwardMovement = (int)get_key(keys, SDLK_w).isPressed - (int)get_key(keys, SDLK_s).isPressed;
    player->pos = vector_add(player->pos, vector_mult_scalar(player->dir, iForwardMovement * delta * 2.f));

    int iSideMovement = (int)get_key(keys, SDLK_d).isPressed - (int)get_key(keys, SDLK_a).isPressed;
    player->pos = vector_add(player->pos, vector_mult_scalar(vector_right(player->dir), iSideMovement * delta * 2.f));

    player->bJustAttacked = false;

    if (get_key(keys, SDLK_SPACE).isPressed)
    {
        if (player->bIsShooting == false)
        {
            player->bIsShooting = true;
            player->iGunFrame = 1;
            player->bJustAttacked = true;
        }
    }

    if (player->bIsShooting)
    {
        player->fGunAnimTimeCounter += delta;
        if (player->fGunAnimTimeCounter >= GUN_ANIM_DELAY)
        {
            player->iGunFrame++;
            player->fGunAnimTimeCounter = 0.0f;
            if (player->iGunFrame >= 5)
            {
                player->iGunFrame = 0;
                player->bIsShooting = false;
            }
        }
    }
    else
    {
        if (get_key(keys, SDLK_UP).justPressed)
        {
            player->selectedWeapon--;
            if (player->selectedWeapon < 0)
                player->selectedWeapon = 3 - 1;
        }
        else if (get_key(keys, SDLK_DOWN).justPressed)
        {

            player->selectedWeapon++;
            if (player->selectedWeapon > 3 - 1)
                player->selectedWeapon = 0;
        }
    }

    player->circle.p.x = player->pos.x;
    player->circle.p.y = player->pos.y;
    if ((iForwardMovement != 0 || iSideMovement != 0) || fabs(player->fOffsetVertical) > HEAD_BOB_AMPLITUDE * 0.1f)
    { // head bobbing
        player->fVerticalAngle += PI * HEAD_BOB_SPEED * delta;
        if (player->fVerticalAngle > PI * 2)
            player->fVerticalAngle = 0.0f;
        player->fOffsetVertical = HEAD_BOB_AMPLITUDE * sin(player->fVerticalAngle);
    }

    if (player->fHitAnimOpacity >= 0.0f)
    {
        player->fHitAnimOpacity -= delta / player->fHitAnimOpacity;
    }
}

void init_player(player_t *player)
{
    player->pos.x = 5.6f;
    player->pos.y = 8.5f;
    player->dir.x = 0;
    player->dir.y = -1.0f;
    player->plane.x = 0.66f;
    player->plane.y = 0;
    player->iGunFrame = 0;
    player->bIsShooting = false;
    player->fGunAnimTimeCounter = 0.0f;
    player->circle.p.x = player->pos.x;
    player->circle.p.y = player->pos.y;
    player->circle.r = 0.25f; // 0.25f
    player->fOffsetVertical = 7.0f;
    player->fVerticalAngle = 0.0f;
    //player->health = PLAYER_MAX_HEALTH;
    player->health = 2;
    player->isDead = false;
    player->fHitAnimOpacity = -1.0f;
    player->selectedWeapon = 0;
}

void player_take_hit(player_t *player)
{
    player->health--;
    if (player->health <= 0)
        player->isDead = true;
    else
        player->fHitAnimOpacity = 1.0f;
}

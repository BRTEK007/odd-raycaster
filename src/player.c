#include "player.h"

void Player_update(Player *player, KeysArray *keys, float delta)
{
    int iRotationMovement = (int)getKeyData(keys, SDLK_RIGHT).isPressed - (int)getKeyData(keys, SDLK_LEFT).isPressed;
    if (iRotationMovement != 0)
    {
        float a = PI * 0.5f * delta * iRotationMovement;
        float cosA = cos(a);
        float sinA = sin(a);
        Vector2f v = player->dir;
        player->dir.x = v.x * cosA + v.y * (-sinA);
        player->dir.y = v.x * sinA + v.y * cosA;

        Vector2f v2 = player->plane;
        player->plane.x = v2.x * cosA + v2.y * (-sinA);
        player->plane.y = v2.x * sinA + v2.y * cosA;
    }

    int iForwardMovement = (int)getKeyData(keys, SDLK_w).isPressed - (int)getKeyData(keys, SDLK_s).isPressed;
    player->pos = Vector2f_add(player->pos, Vector2f_multScalar(player->dir, iForwardMovement * delta * 2.f));

    int iSideMovement = (int)getKeyData(keys, SDLK_d).isPressed - (int)getKeyData(keys, SDLK_a).isPressed;
    player->pos = Vector2f_add(player->pos, Vector2f_multScalar(Vector2f_right(player->dir), iSideMovement * delta * 2.f));

    player->isJustAttacked = false;

    if (getKeyData(keys, SDLK_SPACE).isPressed)
    {
        if (player->isShooting == false)
        {
            player->isShooting = true;
            player->gunFrame = 1;
            player->isJustAttacked = true;
        }
    }

    if (player->isShooting)
    {
        player->gunAnimTimeCounter += delta;
        if (player->gunAnimTimeCounter >= GUN_ANIM_DELAY)
        {
            player->gunFrame++;
            player->gunAnimTimeCounter = 0.0f;
            if (player->gunFrame >= 5)
            {
                player->gunFrame = 0;
                player->isShooting = false;
            }
        }
    }
    else
    {
        if (getKeyData(keys, SDLK_UP).isJustPressed)
        {
            player->selectedWeapon--;
            if (player->selectedWeapon < 0)
                player->selectedWeapon = 3 - 1;
        }
        else if (getKeyData(keys, SDLK_DOWN).isJustPressed)
        {

            player->selectedWeapon++;
            if (player->selectedWeapon > 3 - 1)
                player->selectedWeapon = 0;
        }
    }

    player->circle.p.x = player->pos.x;
    player->circle.p.y = player->pos.y;
    if ((iForwardMovement != 0 || iSideMovement != 0) || fabs(player->offsetVertical) > HEAD_BOB_AMPLITUDE * 0.1f)
    { // head bobbing
        player->verticalAngle += PI * HEAD_BOB_SPEED * delta;
        if (player->verticalAngle > PI * 2)
            player->verticalAngle = 0.0f;
        player->offsetVertical = HEAD_BOB_AMPLITUDE * sin(player->verticalAngle);
    }

    if (player->hitAnimOpacity >= 0.0f)
    {
        player->hitAnimOpacity -= delta / player->hitAnimOpacity;
    }
}

void Player_init(Player *player)
{
    player->pos.x = 5.6f;
    player->pos.y = 8.5f;
    player->dir.x = 0;
    player->dir.y = -1.0f;
    player->plane.x = 0.66f;
    player->plane.y = 0;
    player->gunFrame = 0;
    player->isShooting = false;
    player->gunAnimTimeCounter = 0.0f;
    player->circle.p.x = player->pos.x;
    player->circle.p.y = player->pos.y;
    player->circle.r = 0.25f; // 0.25f
    player->offsetVertical = 7.0f;
    player->verticalAngle = 0.0f;
    //player->health = PLAYER_MAX_HEALTH;
    player->health = 2;
    player->isDead = false;
    player->hitAnimOpacity = -1.0f;
    player->selectedWeapon = 0;
}

void Player_takeHit(Player *player)
{
    player->health--;
    if (player->health <= 0)
        player->isDead = true;
    else
        player->hitAnimOpacity = 1.0f;
}

#include "drawRaycast.h"

/*
Copyright (c) 2004-2020, Lode Vandevenne
Copyright (c) 2022, BRTEK007

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

void drawFloorNCeiling(SDL_Surface *screenSurface, Player *player, SDL_Surface *walls_surface, int startY, int endY)
{
    for (int y = startY; y < endY; y++)
    {
        bool bIsFloor = y > SCREEN_HEIGHT / 2;

        // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
        float rayDirX0 = player->dir.x - player->plane.x;
        float rayDirY0 = player->dir.y - player->plane.y;
        float rayDirX1 = player->dir.x + player->plane.x;
        float rayDirY1 = player->dir.y + player->plane.y;

        // Current y position compared to the center of the screen (the horizon)
        int p = bIsFloor ? (y - SCREEN_HEIGHT / 2) : (SCREEN_HEIGHT / 2 - y);

        // Vertical position of the camera.
        float camZ = bIsFloor ? (0.5 * SCREEN_HEIGHT + player->offsetVertical) : (0.5 * SCREEN_HEIGHT - player->offsetVertical);

        // Horizontal distance from the camera to the floor for the current row.
        // 0.5 is the z position exactly in the middle between floor and ceiling.
        float rowDistance = camZ / p;

        // calculate the real world step vector we have to add for each x (parallel to camera plane)
        // adding step by step avoids multiplications with a weight in the inner loop
        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

        // real world coordinates of the leftmost column. This will be updated as we step to the right.
        float floorX = player->pos.x + rowDistance * rayDirX0;
        float floorY = player->pos.y + rowDistance * rayDirY0;

        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            // the cell coord is simply got from the integer parts of floorX and floorY
            int cellX = (int)(floorX);
            int cellY = (int)(floorY);

            // get the texture coordinate from the fractional part
            int tx = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
            int ty = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            Uint8 r, g, b, a;
            float light = (PLAYER_LIGHT) / distanceSquared(player->pos.x, player->pos.y, floorX, floorY);
            if (light > 1.0f)
                light = 1.0f;

            if (bIsFloor)
            {
                Uint32 pixelFloor = getPixelFast(walls_surface, tx + (TEXTURE_SIZE + 1) * 1, ty);
                SDL_GetRGBA(pixelFloor, walls_surface->format, &r, &g, &b, &a);
                pixelFloor = SDL_MapRGBA(walls_surface->format, r * light, g * light, b * light, a);
                setPixelFast(screenSurface, x, y, pixelFloor);
            }
            else
            {
                Uint32 pixelCeiling = getPixelFast(walls_surface, tx + (TEXTURE_SIZE + 1) * 2, ty);
                SDL_GetRGBA(pixelCeiling, walls_surface->format, &r, &g, &b, &a);
                pixelCeiling = SDL_MapRGBA(walls_surface->format, r * light, g * light, b * light, a);
                setPixelFast(screenSurface, x, y, pixelCeiling);
            }
        }
    }
}

void drawWalls(SDL_Surface *screenSurface, Player *player, Ray *rays, float *fZBuffer, SDL_Surface *walls_surface, int startX, int endX)
{
    float fPlayerCameraDistance = Vector2f_mag(player->dir);

    for (int x = startX; x < endX; x++)
    {
        if (rays[x].isHit)
        {
            float fHeadPositionCorrection = 0.0f;
            fHeadPositionCorrection = player->offsetVertical / rays[x].correctedDistance;

            int iLineHeight = (int)floor(SCREEN_HEIGHT * fPlayerCameraDistance / rays[x].correctedDistance);
            int iStartY = SCREEN_HEIGHT / 2 - iLineHeight / 2 + fHeadPositionCorrection;
            int iEndY = SCREEN_HEIGHT / 2 + iLineHeight / 2 + fHeadPositionCorrection;
            float fWallX = (rays[x].blockSide == VERTICAL) ? rays[x].hitPos.x : rays[x].hitPos.y;
            fWallX -= floor(fWallX);

            const int texture_width = rays[x].isDiagonal ? ((int)(TEXTURE_SIZE * SQUARE_ROOT_OF_TWO)) : TEXTURE_SIZE;

            int tx = fWallX * texture_width; // texture width
            if (rays[x].blockSide == HORIZONTAL && rays[x].dir.x > 0)
                tx = texture_width - tx - 1; // texture width
            if (rays[x].blockSide == VERTICAL && rays[x].dir.y < 0)
                tx = texture_width - tx - 1;                // texture width
            tx += (texture_width + 1) * rays[x].textureId; // offset to different texture

            if (iStartY < 0)
                iStartY = 0;
            if (iEndY > SCREEN_HEIGHT)
                iEndY = SCREEN_HEIGHT;

            float step = 1.0f * TEXTURE_SIZE / iLineHeight; // 64-> texture height
            double texPos = (iStartY - SCREEN_HEIGHT / 2 + iLineHeight / 2 - fHeadPositionCorrection) * step;
            const int ty_offset = rays[x].isDiagonal ? TEXTURE_SIZE + 1 : 0;

            float light = (PLAYER_LIGHT) / (rays[x].correctedDistance * rays[x].correctedDistance);
            if (light > 1.0f)
                light = 1.0f;
            // if(light < 0.25f) light = 0.25f;

            for (int y = iStartY; y < iEndY; y++)
            {
                int ty = (int)texPos & (TEXTURE_SIZE - 1); // 64 -> texture height
                texPos += step;
                Uint32 pixel = getPixelFast(walls_surface, tx, ty + ty_offset);
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, walls_surface->format, &r, &g, &b, &a);
                pixel = SDL_MapRGBA(walls_surface->format, r * light, g * light, b * light, a);
                setPixelFast(screenSurface, x, y, pixel);
            }

            fZBuffer[x] = rays[x].distance;
        }
    }
}


void drawSprites(SDL_Renderer *renderer, Player *player, SpriteArray *sprites, float *fZBuffer, SDL_Texture *soldier_texture, ScalingData *scalingInfo)
{
    qsort(sprites->arr, sprites->size, sizeof(Sprite), Sprite_compareDistance);

    SDL_Rect SrcR;
    SDL_Rect DestR;

    // const Uint32 transparentPixel = SDL_MapRGBA(soldier_surface->format, 255, 255, 255, 0);
    // printf("%u %u \n", transparentPixel, get_pixel_fast(soldier_surface, 0,0));

    for (int i = 0; i < sprites->size; i++)
    {
        // enemy_t *zombie = &enemies->enemies[i];
        Sprite *zombie = &(sprites->arr[i]);
        float spriteX = zombie->pos.x - player->pos.x;
        float spriteY = zombie->pos.y - player->pos.y;

        float invDet = 1.0 / (player->plane.x * player->dir.y - player->dir.x * player->plane.y);
        float transformX = invDet * (player->dir.y * spriteX - player->dir.x * spriteY);
        float transformY = invDet * (-player->plane.y * spriteX + player->plane.x * spriteY);

        if (transformY <= 0)
            continue; // sprite behind camera

        int spriteScreenX = (int)((SCREEN_WIDTH / 2) * (1.0 + transformX / transformY));

#define uDiv 1 // scale x
#define vDiv 1 // scale y
#define vMove 0
        int vMoveScreen = (int)(vMove / transformY);

        vMoveScreen = (int)(vMove / transformY) + player->offsetVertical / transformY;

        int spriteHeight = abs((int)(SCREEN_HEIGHT / (transformY))) / vDiv;
        int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2 + vMoveScreen;
        if (drawStartY < 0)
            drawStartY = 0;
        int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2 + vMoveScreen;
        if (drawEndY >= SCREEN_HEIGHT)
            drawEndY = SCREEN_HEIGHT - 1;

        int spriteWidth = abs((int)(SCREEN_HEIGHT / (transformY))) / uDiv;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= SCREEN_WIDTH)
            drawEndX = SCREEN_WIDTH - 1;

        // if(i == 0){
        int l = drawEndX;
        int r = drawStartX;

        for (int x = drawStartX; x < drawEndX; x++)
        {
            if (transformY < fZBuffer[x])
            {
                l = x;
                break;
            }
        }

        for (int x = drawEndX - 1; x >= drawStartX; x--)
        {
            if (transformY < fZBuffer[x])
            {
                r = x;
                break;
            }
        }

        drawStartX = l;
        drawEndX = r;

        if (drawStartX > drawEndX)
        { // sprite behind wall
            continue;
        }

        const int texture_width = zombie->tw;
        int texXLeft = (int)(256 * (drawStartX - (-spriteWidth / 2 + spriteScreenX)) * texture_width / spriteWidth) / 256;
        int texXRight = (int)(256 * (drawEndX - (-spriteWidth / 2 + spriteScreenX)) * texture_width / spriteWidth) / 256;

        float light = (PLAYER_LIGHT) / (zombie->distanceToPlayerSquared);
        if (light > 1.0f)
            light = 1.0f;

        SrcR.x = zombie->tx + texXLeft;
        SrcR.y = zombie->ty;
        SrcR.w = texXRight - texXLeft;
        SrcR.h = zombie->th;
        DestR.x = scalingInfo->offsetX + (drawStartX + GAME_SCREEN_OFFSET_X) * scalingInfo->scalingFactor;
        DestR.y = scalingInfo->offsetY + (SCREEN_HEIGHT / 2 - spriteHeight / 2 + vMoveScreen + GAME_SCREEN_OFFSET_Y) * scalingInfo->scalingFactor;
        DestR.w = (drawEndX - drawStartX) * scalingInfo->scalingFactor;
        DestR.h = (spriteHeight)*scalingInfo->scalingFactor;

        SDL_SetTextureColorMod(soldier_texture, light * 255, light * 255, light * 255);
        SDL_RenderCopy(renderer, soldier_texture, &SrcR, &DestR);
    }
}


int Sprite_compareDistance(const void *a, const void *b)
{
    if (((Sprite *)a)->distanceToPlayerSquared < ((Sprite *)b)->distanceToPlayerSquared)
        return 1;
    return -1;
}
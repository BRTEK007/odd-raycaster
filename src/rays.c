#include "rays.h"

void getSegmentFromDiagonalWall(char blockType, Vector2f *segA, Vector2f *segB)
{
    if (blockType == 'A')
    {
        *segA = (Vector2f){.x = 0, .y = 0 + 1};
        *segB = (Vector2f){.x = 0 + 1, .y = 0};
    }
    else if (blockType == 'B')
    {
        *segA = (Vector2f){.x = 0, .y = 0};
        *segB = (Vector2f){.x = 0 + 1, .y = 0 + 1};
    }
    else if (blockType == 'C')
    {
        *segA = (Vector2f){.x = 0, .y = 0};
        *segB = (Vector2f){.x = 0 + 1, .y = 0 + 1};
    }
    else if (blockType == 'D')
    {
        *segA = (Vector2f){.x = 0, .y = 0 + 1};
        *segB = (Vector2f){.x = 0 + 1, .y = 0};
    }
}

bool didRayHitDiagonalWall(Ray ray, char blockType)
{
    if (blockType == 'A')
    {
        // check for ray direction, if correct set bBasicWallCollision true, else set segA and segB
        if ((ray.blockSide == HORIZONTAL && ray.dir.x < 0) || (ray.blockSide == VERTICAL && ray.dir.y < 0))
            return true;
        return false;
    }
    else if (blockType == 'B')
    {
        if ((ray.blockSide == HORIZONTAL && ray.dir.x > 0) || (ray.blockSide == VERTICAL && ray.dir.y < 0))
            return true;
        return false;
    }
    else if (blockType == 'C')
    {
        if ((ray.blockSide == HORIZONTAL && ray.dir.x < 0) || (ray.blockSide == VERTICAL && ray.dir.y > 0))
            return true;
        return false;
    }
    else if (blockType == 'D')
    {
        if ((ray.blockSide == HORIZONTAL && ray.dir.x > 0) || (ray.blockSide == VERTICAL && ray.dir.y > 0))
            return true;
        return false;
    }

    return true;
}

void castRays(Player *player, Ray *rays, Map *map)
{
    unsigned iRayCount = 0;

    float fPlayerCameraDistance = Vector2f_mag(player->dir);
    float fPlayerCameraWidth = Vector2f_mag(player->plane);

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        Ray ray;
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1 + (1.0f / (float)SCREEN_WIDTH) / 2.0f;

        Vector2f rayStart = player->pos;
        Vector2f rayDir;
        rayDir.x = player->dir.x + player->plane.x * cameraX;
        rayDir.y = player->dir.y + player->plane.y * cameraX;
        rayDir = Vector2f_norm(rayDir);
        Vector2f rayUnitStepSize;

        ray.dir.x = rayDir.x;
        ray.dir.y = rayDir.y;

        // check for horizontal and vertical rays -> division by 0
        rayUnitStepSize.x = sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x));
        rayUnitStepSize.y = sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y));

        int mapX = (int)floor(rayStart.x);
        int mapY = (int)floor(rayStart.y);

        Vector2f rayLength1D;

        int stepX, stepY;

        if (rayDir.x < 0)
        {
            stepX = -1;
            rayLength1D.x = (rayStart.x - (float)mapX) * rayUnitStepSize.x;
        }
        else
        {
            stepX = 1;
            rayLength1D.x = ((float)(mapX + 1) - rayStart.x) * rayUnitStepSize.x;
        }
        if (rayDir.y < 0)
        {
            stepY = -1;
            rayLength1D.y = (rayStart.y - (float)mapY) * rayUnitStepSize.y;
        }
        else
        {
            stepY = 1;
            rayLength1D.y = ((float)(mapY + 1) - rayStart.y) * rayUnitStepSize.y;
        }

        bool hit = false;
        // float maxDistance = MAP_HEIGHT*MAP_WIDTH;
        float maxDistance = 100.0f;
        float distance = 0.0f;

        ////////
        if (map->arr[mapY][mapX].blockType != '#' && map->arr[mapY][mapX].blockType != 'E')
        {
            Vector2f segA, segB;
            getSegmentFromDiagonalWall(map->arr[mapY][mapX].blockType, &segA, &segB);
            segA.x += mapX;
            segA.y += mapY;
            segB.x += mapX;
            segB.y += mapY;

            Vector2f inter;
            if (raySegCollision(rayStart, rayDir, segA, segB, &inter))
            {
                if (Vector2f_dot(player->dir, Vector2f_sub(inter, player->pos)) >= 0.f)
                {
                    hit = true;
                    ray.hitPos = inter;
                    ray.distance = Vector2f_mag(Vector2f_sub(rayStart, inter));
                    ray.textureId = 0;
                    ray.blockId = mapX + mapY * map->width;
                    float fAngle = atan2(fPlayerCameraWidth * cameraX, fPlayerCameraDistance);
                    ray.correctedDistance = cos(fAngle) * ray.distance;
                    ray.isDiagonal = true;
                    // ray.blockSide = HORIZONTAL;
                }
            }
        }
        ///////

        while (!hit && distance < maxDistance)
        {

            if (rayLength1D.x < rayLength1D.y)
            {
                mapX += stepX;
                distance = rayLength1D.x;
                rayLength1D.x += rayUnitStepSize.x;
                ray.blockSide = HORIZONTAL;
            }
            else
            {
                mapY += stepY;
                distance = rayLength1D.y;
                rayLength1D.y += rayUnitStepSize.y;
                ray.blockSide = VERTICAL;
            }
            if (mapY < 0 || mapX < 0 || mapY >= map->height || mapX >= map->width)
                continue;

            if (map->arr[mapY][mapX].blockType != '#')
            {
                bool bBasicWallCollision = didRayHitDiagonalWall(ray, map->arr[mapY][mapX].blockType);

                if (bBasicWallCollision)
                {
                    hit = true;
                    ray.hitPos = Vector2f_add(rayStart, Vector2f_multScalar(rayDir, distance));
                    ray.distance = distance;
                    ray.textureId = 0;
                    ray.blockId = mapX + mapY * map->width;
                    float fAngle = atan2(fPlayerCameraWidth * cameraX, fPlayerCameraDistance);
                    ray.correctedDistance = cos(fAngle) * distance;
                    ray.isDiagonal = false;
                }
                else
                { // check for collision with segment
                    Vector2f inter;

                    Vector2f segA, segB;

                    getSegmentFromDiagonalWall(map->arr[mapY][mapX].blockType, &segA, &segB);
                    segA.x += mapX;
                    segA.y += mapY;
                    segB.x += mapX;
                    segB.y += mapY;
                    if (raySegCollision(rayStart, rayDir, segA, segB, &inter))
                    {
                        hit = true;
                        ray.hitPos = inter;
                        ray.distance = Vector2f_mag(Vector2f_sub(rayStart, inter));
                        ray.textureId = 0;
                        ray.blockId = mapX + mapY * map->width;
                        float fAngle = atan2(fPlayerCameraWidth * cameraX, fPlayerCameraDistance);
                        ray.correctedDistance = cos(fAngle) * ray.distance;
                        ray.isDiagonal = true;
                        // ray.blockSide = HORIZONTAL;
                    }
                }
            }
        }

        ray.isHit = hit;
        rays[iRayCount++] = ray;
    }
}

Ray castOneRay(Vector2f rayStart, Vector2f rayDir, Map *map)
{
    Ray ray;

    rayDir = Vector2f_norm(rayDir);
    Vector2f rayUnitStepSize;

    ray.dir.x = rayDir.x;
    ray.dir.y = rayDir.y;

    // check for horizontal and vertical rays -> division by 0
    rayUnitStepSize.x = sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x));
    rayUnitStepSize.y = sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y));

    int mapX = (int)floor(rayStart.x);
    int mapY = (int)floor(rayStart.y);

    Vector2f rayLength1D;

    int stepX, stepY;

    if (rayDir.x < 0)
    {
        stepX = -1;
        rayLength1D.x = (rayStart.x - (float)mapX) * rayUnitStepSize.x;
    }
    else
    {
        stepX = 1;
        rayLength1D.x = ((float)(mapX + 1) - rayStart.x) * rayUnitStepSize.x;
    }
    if (rayDir.y < 0)
    {
        stepY = -1;
        rayLength1D.y = (rayStart.y - (float)mapY) * rayUnitStepSize.y;
    }
    else
    {
        stepY = 1;
        rayLength1D.y = ((float)(mapY + 1) - rayStart.y) * rayUnitStepSize.y;
    }

    bool hit = false;
    // float maxDistance = MAP_HEIGHT*MAP_WIDTH;
    float maxDistance = 100.0f;
    float distance = 0.0f;

    ////////
    if (map->arr[mapY][mapX].blockType != '#' && map->arr[mapY][mapX].blockType != 'E')
    {
        Vector2f segA, segB;
        getSegmentFromDiagonalWall(map->arr[mapY][mapX].blockType, &segA, &segB);
        segA.x += mapX;
        segA.y += mapY;
        segB.x += mapX;
        segB.y += mapY;

        Vector2f inter;
        if (raySegCollision(rayStart, rayDir, segA, segB, &inter))
        {
            if (Vector2f_dot(rayDir, Vector2f_sub(inter, rayStart)) >= 0.f)
            {
                hit = true;
                ray.hitPos = inter;
                ray.distance = Vector2f_mag(Vector2f_sub(rayStart, inter));
                ray.textureId = 0;
                ray.blockId = mapX + mapY * map->width;
                float fAngle = 0.0f;
                ray.correctedDistance = cos(fAngle) * ray.distance;
                ray.isDiagonal = true;
                // ray.blockSide = HORIZONTAL;
            }
        }
    }
    ///////

    while (!hit && distance < maxDistance)
    {

        if (rayLength1D.x < rayLength1D.y)
        {
            mapX += stepX;
            distance = rayLength1D.x;
            rayLength1D.x += rayUnitStepSize.x;
            ray.blockSide = HORIZONTAL;
        }
        else
        {
            mapY += stepY;
            distance = rayLength1D.y;
            rayLength1D.y += rayUnitStepSize.y;
            ray.blockSide = VERTICAL;
        }
        if (mapY < 0 || mapX < 0 || mapY >= map->height || mapX >= map->width)
            continue;

        if (map->arr[mapY][mapX].blockType != '#')
        {
            bool bBasicWallCollision = didRayHitDiagonalWall(ray, map->arr[mapY][mapX].blockType);

            if (bBasicWallCollision)
            {
                hit = true;
                ray.hitPos = Vector2f_add(rayStart, Vector2f_multScalar(rayDir, distance));
                ray.distance = distance;
                ray.textureId = 0;
                ray.blockId = mapX + mapY * map->width;
                float fAngle = 0.0f;
                ray.correctedDistance = cos(fAngle) * distance;
                ray.isDiagonal = false;
            }
            else
            { // check for collision with segment
                Vector2f inter;

                Vector2f segA, segB;

                getSegmentFromDiagonalWall(map->arr[mapY][mapX].blockType, &segA, &segB);
                segA.x += mapX;
                segA.y += mapY;
                segB.x += mapX;
                segB.y += mapY;
                if (raySegCollision(rayStart, rayDir, segA, segB, &inter))
                {
                    hit = true;
                    ray.hitPos = inter;
                    ray.distance = Vector2f_mag(Vector2f_sub(rayStart, inter));
                    ray.textureId = 0;
                    ray.blockId = mapX + mapY * map->width;
                    float fAngle = 0.0f;
                    ray.correctedDistance = cos(fAngle) * ray.distance;
                    ray.isDiagonal = true;
                    // ray.blockSide = HORIZONTAL;
                }
            }
        }
    }

    ray.isHit = hit;

    return ray;
}

// returns 1 -> collision, 0 -> no collision, stores collision in inter
int raySegCollision(Vector2f rayStart, Vector2f rayDir, Vector2f segA, Vector2f segB, Vector2f *inter)
{
    float a1, a2, b1, b2;

    a1 = rayDir.y / rayDir.x;
    b1 = rayStart.y - a1 * rayStart.x;

    a2 = (segB.y - segA.y) / (segB.x - segA.x);
    b2 = segB.y - a2 * segB.x;

    if (a1 - a2 == 0.f)
        return 0;

    float x = (b1 - b2) / (a2 - a1);
    float y = a1 * x + b1;
    // check if in bounds of SEG
    float segMinX, segMaxX, segMinY, segMaxY;
    if (segA.x < segB.x)
    {
        segMinX = segA.x;
        segMaxX = segB.x;
    }
    else
    {
        segMinX = segB.x;
        segMaxX = segA.x;
    }
    if (segA.y < segB.y)
    {
        segMinY = segA.y;
        segMaxY = segB.y;
    }
    else
    {
        segMinY = segB.y;
        segMaxY = segA.y;
    }
    if (x > segMaxX || x < segMinX || y > segMaxY || y < segMinY)
        return 0;
    inter->x = x;
    inter->y = y;
    return 1;
}

float rayPointDistSquared(Vector2f rayStart, Vector2f rayDir, Vector2f point)
{
    float a, b;
    a = rayDir.y / rayDir.x;
    b = rayStart.y - a * rayStart.x;
    float t = (b + a * point.x - point.y);
    float distSquared = (t * t) / (a * a + 1);
    return distSquared;
}
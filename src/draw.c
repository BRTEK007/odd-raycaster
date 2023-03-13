#include "draw.h"
#include "drawRaycast.h"

void *draw_floor_threaded(void *vargp)
{
    draw_floor_data_t *data = (draw_floor_data_t *)vargp;
    draw_floor_and_ceiling(data->screenSurface, data->player, data->walls_surface, data->startY, data->endY);
    return NULL;
}

void *draw_walls_threaded(void *vargp)
{
    draw_walls_data_t *data = (draw_walls_data_t *)vargp;
    draw_walls(data->screenSurface, data->player, data->rays, data->zBuffer, data->walls_surface, data->startX, data->endX);
    return NULL;
}

void draw_bar(SDL_Renderer *renderer, player_t *player, SDL_Texture *bar_texture, scaling_info_t *scalingInfo)
{
    SDL_Rect SrcR;
    SDL_Rect DestR;
    SrcR.x = 0;
    SrcR.y = 32 + 1;
    SrcR.w = 32;
    SrcR.h = 32;
    DestR.x = scalingInfo->offsetX + (HEARTS_SLOT_X)*scalingInfo->scalingFactor;
    DestR.y = scalingInfo->offsetY + (HEARTS_SLOT_Y)*scalingInfo->scalingFactor;
    DestR.w = (32) * scalingInfo->scalingFactor;
    DestR.h = (32) * scalingInfo->scalingFactor;

    // HEARTS
    for (int i = 0; i < player->health / 2; i++)
    {
        DestR.x = scalingInfo->offsetX + (HEARTS_SLOT_X + (32) * i) * scalingInfo->scalingFactor;
        SDL_RenderCopy(renderer, bar_texture, &SrcR, &DestR);
    }

    if (player->health % 2 == 1)
    {
        SrcR.x = 32 + 1;
        DestR.x = scalingInfo->offsetX + (HEARTS_SLOT_X + (32) * (player->health / 2)) * scalingInfo->scalingFactor;
        SDL_RenderCopy(renderer, bar_texture, &SrcR, &DestR);
    }

    // HEAD
    SrcR.x = ((PLAYER_MAX_HEALTH - player->health) / 2) * (32 + 1);
    SrcR.y = 0;
    DestR.x = scalingInfo->offsetX + (HEAD_SLOT_X)*scalingInfo->scalingFactor;
    DestR.y = scalingInfo->offsetY + (HEAD_SLOT_Y)*scalingInfo->scalingFactor;
    SDL_RenderCopy(renderer, bar_texture, &SrcR, &DestR);

    // weapon
    SrcR.x = (64 + 1) * player->selectedWeapon;
    SrcR.w = 64;
    SrcR.y = (32 + 1) * 2;
    DestR.x = scalingInfo->offsetX + (WEAPON_SLOT_X)*scalingInfo->scalingFactor;
    DestR.y = scalingInfo->offsetY + (WEAPON_SLOT_Y)*scalingInfo->scalingFactor;
    DestR.w = 64 * scalingInfo->scalingFactor;
    SDL_RenderCopy(renderer, bar_texture, &SrcR, &DestR);
}

void draw_text(SDL_Renderer *renderer, SDL_Texture *texture, int sx, int sy, char *text)
{
    SDL_Rect SrcR;
    SDL_Rect DestR;

    for (int i = 0; text[i] != '\0'; i++)
    {
        int n = text[i] - 'A';
        SrcR.x = n * 3 + n;
        SrcR.y = 0;
        SrcR.w = 3;
        SrcR.h = 5;
        DestR.x = sx + i * 12;
        DestR.y = sy;
        DestR.w = 9;
        DestR.h = 15;
        SDL_RenderCopy(renderer, texture, &SrcR, &DestR);
    }
}


void draw_map(SDL_Renderer *renderer, player_t *player, enemy_arr_t *enemies, ray_t *rays, int iRayCount, map_t *map)
{
    /* const int BLOCK_SIZE = 6;
     const int FRAME_SIZE = 1;
     const SDL_Color WALL_COLOR = {0, 0, 255};
     const SDL_Color FRAME_COLOR = {255, 255, 0};

     SDL_Rect squareRect;
     squareRect.x = 0;
     squareRect.y = 0;
     squareRect.w = map->width * BLOCK_SIZE + FRAME_SIZE * 2;
     squareRect.h = map->height * BLOCK_SIZE + FRAME_SIZE * 2;

     SDL_SetRenderDrawColor(renderer, FRAME_COLOR.r, FRAME_COLOR.g, FRAME_COLOR.b, 255);
     SDL_RenderFillRect(renderer, &squareRect);

     squareRect.x = FRAME_SIZE;
     squareRect.y = FRAME_SIZE;
     squareRect.w = map->width * BLOCK_SIZE;
     squareRect.h = map->height * BLOCK_SIZE;

     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
     SDL_RenderFillRect(renderer, &squareRect);

     squareRect.w = BLOCK_SIZE;
     squareRect.h = BLOCK_SIZE;

     SDL_SetRenderDrawColor(renderer, WALL_COLOR.r, WALL_COLOR.g, WALL_COLOR.b, 255);

     for (int x = 0; x < map->width; x++)
     {
         for (int y = 0; y < map->height; y++)
         {
             if (map->arr[y][x].blockType == '#')
                 continue;
             if (map->arr[y][x].blockType == 'E')
             {
                 squareRect.x = FRAME_SIZE + x * BLOCK_SIZE;
                 squareRect.y = FRAME_SIZE + y * BLOCK_SIZE;
                 SDL_RenderFillRect(renderer, &squareRect);
             }
             else
             {
                 SDL_Vertex verts[3];
                 if (map->arr[y][x].blockType == 'A')
                 {
                     verts[0].position = (SDL_FPoint){x, y + 1};
                     verts[1].position = (SDL_FPoint){x + 1, y};
                     verts[2].position = (SDL_FPoint){x + 1, y + 1};
                 }
                 else if (map->arr[y][x].blockType == 'B')
                 {
                     verts[0].position = (SDL_FPoint){x, y};
                     verts[1].position = (SDL_FPoint){x + 1, y + 1};
                     verts[2].position = (SDL_FPoint){x, y + 1};
                 }
                 else if (map->arr[y][x].blockType == 'C')
                 {
                     verts[0].position = (SDL_FPoint){x, y};
                     verts[1].position = (SDL_FPoint){x + 1, y};
                     verts[2].position = (SDL_FPoint){x + 1, y + 1};
                 }
                 else if (map->arr[y][x].blockType == 'D')
                 {
                     verts[0].position = (SDL_FPoint){x, y};
                     verts[1].position = (SDL_FPoint){x + 1, y};
                     verts[2].position = (SDL_FPoint){x, y + 1};
                 }
                 verts[0].color = (SDL_Color){0, 0, 255, 255};
                 verts[1].color = (SDL_Color){0, 0, 255, 255};
                 verts[2].color = (SDL_Color){0, 0, 255, 255};
                 verts[0].position.x = verts[0].position.x * BLOCK_SIZE + FRAME_SIZE;
                 verts[0].position.y = verts[0].position.y * BLOCK_SIZE + FRAME_SIZE;
                 verts[1].position.x = verts[1].position.x * BLOCK_SIZE + FRAME_SIZE;
                 verts[1].position.y = verts[1].position.y * BLOCK_SIZE + FRAME_SIZE;
                 verts[2].position.x = verts[2].position.x * BLOCK_SIZE + FRAME_SIZE;
                 verts[2].position.y = verts[2].position.y * BLOCK_SIZE + FRAME_SIZE;
                 SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
             }
         }
     }

     SDL_Vertex vert[iRayCount + 1];
     int indicies[(iRayCount - 1) * 3];
     unsigned iIndiciesIndex = 0;

     vert[0].position.x = player->pos.x * BLOCK_SIZE + FRAME_SIZE;
     vert[0].position.y = player->pos.y * BLOCK_SIZE + FRAME_SIZE;
     vert[0].color.r = 255;
     vert[0].color.g = 0;
     vert[0].color.b = 255;
     vert[0].color.a = 255;

     for (int i = 1; i < iRayCount; i++)
     {
         indicies[iIndiciesIndex++] = 0;
         indicies[iIndiciesIndex++] = i;
         indicies[iIndiciesIndex++] = i + 1;
     }

     for (int i = 1; i <= iRayCount; i++)
     {
         vert[i].position.x = rays[i - 1].vHit.x * BLOCK_SIZE + FRAME_SIZE;
         vert[i].position.y = rays[i - 1].vHit.y * BLOCK_SIZE + FRAME_SIZE;
         vert[i].color.r = 255;
         vert[i].color.g = 0;
         vert[i].color.b = 255;
         vert[i].color.a = 255;
     }

     SDL_RenderGeometry(renderer, NULL, vert, iRayCount + 1, indicies, (iRayCount - 1) * 3);

     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
     vec2f A = vector_add(player->pos, player->dir);
     SDL_RenderDrawLine(renderer,
                        player->pos.x * BLOCK_SIZE + FRAME_SIZE,
                        player->pos.y * BLOCK_SIZE + FRAME_SIZE,
                        A.x * BLOCK_SIZE + FRAME_SIZE,
                        A.y * BLOCK_SIZE + FRAME_SIZE);

     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
     vec2f B = vector_add(A, player->plane);
     vec2f C = vector_sub(A, player->plane);
     SDL_RenderDrawLine(renderer,
                        B.x * BLOCK_SIZE + FRAME_SIZE,
                        B.y * BLOCK_SIZE + FRAME_SIZE,
                        C.x * BLOCK_SIZE + FRAME_SIZE,
                        C.y * BLOCK_SIZE + FRAME_SIZE);

     // draw entities
     for (int i = 0; i < enemies->size; i++)
     {
         squareRect.x = enemies->enemies[i].pos.x * BLOCK_SIZE;
         squareRect.y = enemies->enemies[i].pos.y * BLOCK_SIZE;
         squareRect.w = BLOCK_SIZE / 2;
         squareRect.h = BLOCK_SIZE / 2;
         // printf("%f %f\n", enemies->enemies[i].pos.x, enemies->enemies[i].pos.y);

         SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
         SDL_RenderFillRect(renderer, &squareRect);
     }*/
}


void draw_weapon(SDL_Renderer *renderer, player_t *player, SDL_Texture *gun_texture, scaling_info_t *scalingInfo)
{
    SDL_Rect SrcR;
    SDL_Rect DestR;

    SrcR.x = player->iGunFrame * 64 + player->iGunFrame;
    SrcR.y = player->selectedWeapon * (64 + 1);
    SrcR.w = 64;
    SrcR.h = 64;
    DestR.x = scalingInfo->offsetX + (SCREEN_WIDTH / 2 - 128 / 2 + GAME_SCREEN_OFFSET_X) * scalingInfo->scalingFactor;
    DestR.y = scalingInfo->offsetY + (SCREEN_HEIGHT - 128 + GAME_SCREEN_OFFSET_Y) * scalingInfo->scalingFactor;
    DestR.w = 128 * scalingInfo->scalingFactor;
    DestR.h = 128 * scalingInfo->scalingFactor;
    SDL_RenderCopy(renderer, gun_texture, &SrcR, &DestR);
}

scaling_info_t get_scaling_info(SDL_Renderer *renderer)
{
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
    scaling_info_t scalingInfo;
    float scalingFactorX = (float)rendererWidth / (FRAME_WIDTH);
    float scalingFactorY = (float)rendererHeight / (FRAME_HEIGHT);
    scalingInfo.scalingFactor = scalingFactorX < scalingFactorY ? (int)floor(scalingFactorX) : (int)floor(scalingFactorY);
    scalingInfo.offsetX = (rendererWidth - FRAME_WIDTH * scalingInfo.scalingFactor) / 2;
    scalingInfo.offsetY = (rendererHeight - FRAME_HEIGHT * scalingInfo.scalingFactor) / 2;
    scalingInfo.rendererHeight = rendererHeight;
    scalingInfo.rendererWidth = rendererWidth;
    return scalingInfo;
}

void text_texture_free(text_texture_t *tt)
{
    SDL_DestroyTexture(tt->texture);
}

void draw_paused_bar(SDL_Renderer *renderer, TTF_Font *font, scaling_info_t *scalingInfo, int selectedOption)
{
    SDL_Rect rect;
    // draw bottom background
    rect.x = scalingInfo->offsetX + BAR_SCREEN_OFFSET_X * scalingInfo->scalingFactor;
    rect.y = scalingInfo->offsetY + BAR_SCREEN_OFFSET_Y * scalingInfo->scalingFactor;
    rect.w = BAR_SCREEN_WIDTH * scalingInfo->scalingFactor;
    rect.h = BAR_SCREEN_HEIGHT * scalingInfo->scalingFactor;
    SDL_SetRenderDrawColor(renderer, 0, 25, 64, 255);
    SDL_RenderFillRect(renderer, &rect);
    // draw bottom text
    text_texture_t textPaused = create_text_texture(renderer, font, "--PAUSED--", (SDL_Color){255, 225, 191});
    rect.w = textPaused.aspectRatio * rect.h;
    rect.x = scalingInfo->offsetX + BAR_SCREEN_OFFSET_X * scalingInfo->scalingFactor + BAR_SCREEN_WIDTH * scalingInfo->scalingFactor / 2 - rect.w / 2;
    SDL_RenderCopy(renderer, textPaused.texture, NULL, &rect);
    text_texture_free(&textPaused);

    // draw top background
    rect.x = scalingInfo->offsetX + GAME_SCREEN_OFFSET_X * scalingInfo->scalingFactor;
    rect.y = scalingInfo->offsetY + GAME_SCREEN_OFFSET_Y * scalingInfo->scalingFactor;
    rect.w = SCREEN_WIDTH * scalingInfo->scalingFactor;
    rect.h = SCREEN_HEIGHT * scalingInfo->scalingFactor;
    SDL_RenderFillRect(renderer, &rect);

// draw top text
#define OPTIONS_COUNT 2
#define TEXT_MARGIN_RATIO 2
    const static char *options[OPTIONS_COUNT] = {"RESUME", "MENU"};
    const static char *optionsSelected[OPTIONS_COUNT] = {">RESUME<", ">MENU<"};

    const int textHeight = SCREEN_HEIGHT / 4;

    for (int i = 0; i < OPTIONS_COUNT; i++)
    {
        const char *text = i == selectedOption ? optionsSelected[i] : options[i];

        text_texture_t textTexture = create_text_texture(renderer, font, text, (SDL_Color){255, 225, 191});

        int textWidth = textHeight * textTexture.aspectRatio;

        rect.x = scalingInfo->rendererWidth / 2 - textWidth / 2;
        rect.y = scalingInfo->rendererHeight / 2 + i * textHeight * TEXT_MARGIN_RATIO - (OPTIONS_COUNT * textHeight * TEXT_MARGIN_RATIO) / 2;
        rect.w = textWidth;
        rect.h = textHeight;

        SDL_RenderCopy(renderer, textTexture.texture, NULL, &rect);
        text_texture_free(&textTexture);
    }
#undef OPTIONS_COUNT
#undef TEXT_MARGIN_RATIO
}

text_texture_t create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color)
{
    text_texture_t tt;
    SDL_Surface *surf = TTF_RenderText_Solid(font, text, color);
    tt.texture = SDL_CreateTextureFromSurface(renderer, surf);
    tt.aspectRatio = surf->w / surf->h;
    SDL_FreeSurface(surf);
    return tt;
}
#include "gameState.h"
#include <stdio.h>
#include "fizzlefade.h"

#ifdef THREADED_FLOOR_RENDERING
#include <pthread.h>
#endif

void gameState_init(gameState_t *gameState, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_Surface *Loading_Surf;

    Loading_Surf = SDL_LoadBMP("../assets/walls.bmp");
    gameState->walls_texture = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);

    Loading_Surf = SDL_LoadBMP("../assets/bar.bmp");
    SDL_SetColorKey(Loading_Surf, SDL_TRUE, SDL_MapRGB(Loading_Surf->format, 255, 0, 255));
    gameState->bar_texture = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);

    Loading_Surf = SDL_LoadBMP("../assets/enemy.bmp");
    SDL_SetColorKey(Loading_Surf, SDL_TRUE, SDL_MapRGB(Loading_Surf->format, 255, 0, 255));
    gameState->soldier_texture = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);

    Loading_Surf = SDL_LoadBMP("../assets/gun.bmp");
    SDL_SetColorKey(Loading_Surf, SDL_TRUE, SDL_MapRGB(Loading_Surf->format, 255, 0, 255));
    gameState->gun_texture = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);

    Loading_Surf = SDL_LoadBMP("../assets/screen.bmp");
    SDL_SetColorKey(Loading_Surf, SDL_TRUE, SDL_MapRGB(Loading_Surf->format, 255, 0, 255));
    gameState->frame_texture = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);

    gameState->walls_surface = SDL_LoadBMP("../assets/walls.bmp");
    gameState->soldier_surface = SDL_LoadBMP("../assets/soldier.bmp");

    gameState->youDiedText = create_text_texture(renderer, font, "YOU DIED", (SDL_Color){237, 0, 3});
    gameState->font = font;

    ///////
    init_player(&(gameState->player));
    init_keys(&(gameState->keys));
    init_enemy_arr(&(gameState->enemies), 1);

    for (int i = 0; i < 1; i++)
        add_enemy(&(gameState->enemies), 2 + i * 2.f, 2);

    init_procetile_arr(&(gameState->enemy_projectiles), 32);
    // add_procetile(&(gameState->enemy_projectiles), 6, 6);

    init_map(&gameState->map);

    gameState->rays = calloc(SCREEN_WIDTH, sizeof(ray_t));
    if (gameState->rays == NULL)
        exit(1);

    gameState->screenSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_SetSurfaceRLE(gameState->screenSurface, 1);

    // VIGNETE
    SDL_Surface *vignete_surf = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    memset(vignete_surf->pixels, 255, SCREEN_HEIGHT * SCREEN_WIDTH * vignete_surf->format->BytesPerPixel);
    SDL_SetColorKey(vignete_surf, SDL_TRUE, SDL_MapRGB(vignete_surf->format, 255, 255, 255));
    gameState->vignete_texture = SDL_CreateTextureFromSurface(renderer, vignete_surf);
    // for(int i = 0;)
    SDL_FreeSurface(vignete_surf);
    //

    gameState->deathScreenSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    memset(gameState->deathScreenSurface->pixels, 255, SCREEN_HEIGHT * SCREEN_WIDTH * gameState->deathScreenSurface->format->BytesPerPixel);
    SDL_SetColorKey(gameState->deathScreenSurface, SDL_TRUE, SDL_MapRGB(gameState->deathScreenSurface->format, 255, 255, 255));
    SDL_SetSurfaceRLE(gameState->deathScreenSurface, 1);

    gameState->rndval = 1;
    gameState->deathAnimFinished = false;

    gameState->quit = false;
    gameState->paused = false;
    gameState->selectedOptionPaused = intInRange_create(0, 0, 2 - 1);
}

void gameState_free(gameState_t *gameState)
{
    free_map(&gameState->map);
    free_enemy_arr(&gameState->enemies);
    free(gameState->rays);
    free_procetile_arr(&(gameState->enemy_projectiles));
    text_texture_free(&(gameState->youDiedText));

    SDL_FreeSurface(gameState->screenSurface);
    SDL_DestroyTexture(gameState->walls_texture);
    SDL_DestroyTexture(gameState->soldier_texture);
    SDL_DestroyTexture(gameState->gun_texture);
    SDL_DestroyTexture(gameState->frame_texture);
    SDL_DestroyTexture(gameState->vignete_texture);
    SDL_DestroyTexture(gameState->bar_texture);
    SDL_FreeSurface(gameState->walls_surface);
    SDL_FreeSurface(gameState->soldier_surface);
}

void gameState_event(gameState_t *gameState, SDL_Event event)
{
    read_keyboard(&event, &gameState->keys);
    if (gameState->paused)
    {

        if (get_key(&(gameState->keys), SDLK_RETURN).justPressed)
        {
            switch (gameState->selectedOptionPaused.value)
            {
            case 0:
                gameState->paused = false;
                break;
            case 1:
                gameState->quit = true;
                break;
            }
        }
        else if (get_key(&(gameState->keys), SDLK_UP).justPressed)
        {
            intInRange_decrement(&(gameState->selectedOptionPaused));
        }
        else if (get_key(&(gameState->keys), SDLK_DOWN).justPressed)
        {
            intInRange_increment(&(gameState->selectedOptionPaused));
        }
    }
    else
    {
        if (get_key(&(gameState->keys), SDLK_ESCAPE).justPressed)
        {
            if (!gameState->player.isDead)
                gameState->paused = true;
        }
    }
}

void update_projectiles(projectile_arr_t *projectiles, float delta)
{
    for (int i = 0; i < projectiles->size; i++)
    {
        projectile_t *p = &(projectiles->arr[i]);
        p->pos.x = p->pos.x + p->vDir.x * delta * 3;
        p->pos.y = p->pos.y + p->vDir.y * delta * 3;
        p->circle.p.x = p->pos.x;
        p->circle.p.y = p->pos.y;
        p->fLifetime += delta;
        if (p->fLifetime >= 10.0f)
        {
            projectile_arr_remove(projectiles, i);
            i--;
        }
    }
}

void gameState_draw(gameState_t *gameState, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 25, 255);
    SDL_RenderClear(renderer);

    //****************************************************GAME_SCREEN************************************
    SDL_Rect SrcR;
    SDL_Rect DestR;

    scaling_info_t scalingInfo = get_scaling_info(renderer);

    if (!gameState->paused)
    {

        if (!gameState->player.isDead)
        {
            SDL_LockSurface(gameState->screenSurface);
///
#ifdef THREADED_FLOOR_RENDERING

            pthread_t threads[THREAD_COUNT];
            draw_floor_data_t floor_thread_data[THREAD_COUNT];
            for (int i = 0; i < THREAD_COUNT; i++)
            {
                floor_thread_data[i].player = &(gameState->player);
                floor_thread_data[i].screenSurface = gameState->screenSurface;
                floor_thread_data[i].walls_surface = gameState->walls_surface;
                floor_thread_data[i].startY = i * (SCREEN_HEIGHT / THREAD_COUNT);
                floor_thread_data[i].endY = (i + 1) * (SCREEN_HEIGHT / THREAD_COUNT);
            }
            for (int i = 0; i < THREAD_COUNT; i++)
                pthread_create(&threads[i], NULL, draw_floor_threaded, (void *)&floor_thread_data[i]);
            for (int i = 0; i < THREAD_COUNT; i++)
                pthread_join(threads[i], NULL);

            draw_walls_data_t walls_thread_data[THREAD_COUNT];
            for (int i = 0; i < THREAD_COUNT; i++)
            {
                walls_thread_data[i].player = &(gameState->player);
                walls_thread_data[i].screenSurface = gameState->screenSurface;
                walls_thread_data[i].walls_surface = gameState->walls_surface;
                walls_thread_data[i].rays = gameState->rays;
                walls_thread_data[i].zBuffer = gameState->fZBuffer;
                walls_thread_data[i].startX = i * (SCREEN_WIDTH / THREAD_COUNT);
                walls_thread_data[i].endX = (i + 1) * (SCREEN_WIDTH / THREAD_COUNT);
            }

            for (int i = 0; i < THREAD_COUNT; i++)
                pthread_create(&threads[i], NULL, draw_walls_threaded, (void *)&walls_thread_data[i]);
            for (int i = 0; i < THREAD_COUNT; i++)
                pthread_join(threads[i], NULL);
#else
            draw_floor_and_ceiling(gameState->screenSurface, &gameState->player, gameState->walls_surface, 0, SCREEN_HEIGHT);

            draw_walls(gameState->screenSurface, &gameState->player, gameState->rays, gameState->fZBuffer, gameState->walls_surface, 0, SCREEN_WIDTH);
#endif
            ///

            SDL_UnlockSurface(gameState->screenSurface);
        } //! player is dead

        SrcR.x = 0;
        SrcR.y = 0;
        SrcR.w = SCREEN_WIDTH;
        SrcR.h = SCREEN_HEIGHT;
        DestR.x = GAME_SCREEN_OFFSET_X * scalingInfo.scalingFactor + scalingInfo.offsetX;
        DestR.y = GAME_SCREEN_OFFSET_Y * scalingInfo.scalingFactor + scalingInfo.offsetY;
        DestR.w = SCREEN_WIDTH * scalingInfo.scalingFactor;
        DestR.h = SCREEN_HEIGHT * scalingInfo.scalingFactor;

        SDL_Texture *tTempTexture;

        tTempTexture = SDL_CreateTextureFromSurface(renderer, gameState->screenSurface);
        SDL_RenderCopy(renderer, tTempTexture, &SrcR, &DestR);
        SDL_DestroyTexture(tTempTexture);

        sprite_arr_t sprites;
        sprite_arr_init(&sprites, gameState->enemies.size + gameState->enemy_projectiles.size); // + projectiles->size...

        for (int i = 0; i < gameState->enemies.size; i++)
        {
            sprite_t s = enemy_get_sprite(&(gameState->enemies.enemies[i]));
            s.fDistanceToPlayerSquared = distanceSquared(s.pos.x, s.pos.y, gameState->player.pos.x, gameState->player.pos.y);
            sprite_arr_add(&sprites, s);
        }

        for (int i = 0; i < gameState->enemy_projectiles.size; i++)
        {
            sprite_t s = projectile_get_sprite(&(gameState->enemy_projectiles.arr[i]));
            s.fDistanceToPlayerSquared = distanceSquared(s.pos.x, s.pos.y, gameState->player.pos.x, gameState->player.pos.y);
            sprite_arr_add(&sprites, s);
        }

        draw_sprites(renderer, &gameState->player, &sprites, gameState->fZBuffer, gameState->soldier_texture, &scalingInfo);

        sprite_arr_free(&sprites);

        // draw_map(renderer, &gameState->player, &(gameState->enemies), gameState->rays, SCREEN_WIDTH, &gameState->map);

        draw_weapon(renderer, &gameState->player, gameState->gun_texture, &scalingInfo);

        if (gameState->player.fHitAnimOpacity >= 0) // player hit anim
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200 * gameState->player.fHitAnimOpacity); // hit anim
            SDL_RenderFillRect(renderer, &DestR);                                               // hit anim
        }
    }

    if (gameState->player.isDead) // death animation and deah screen
    {
        SDL_Texture *tTempTexture;
        tTempTexture = SDL_CreateTextureFromSurface(renderer, gameState->deathScreenSurface);
        SDL_RenderCopy(renderer, tTempTexture, &SrcR, &DestR);
        SDL_DestroyTexture(tTempTexture);

        if (gameState->deathAnimFinished)
        {
            int textWidth = (SCREEN_WIDTH * scalingInfo.scalingFactor) / 3;
            int textHeight = (int)((float)textWidth / gameState->youDiedText.aspectRatio);

            DestR.x = GAME_SCREEN_OFFSET_X * scalingInfo.scalingFactor + scalingInfo.offsetX + SCREEN_WIDTH * scalingInfo.scalingFactor * 0.5f - textWidth / 2;
            DestR.y = GAME_SCREEN_OFFSET_Y * scalingInfo.scalingFactor + scalingInfo.offsetY + SCREEN_HEIGHT * scalingInfo.scalingFactor * 0.5f - textHeight / 2;
            DestR.w = textWidth;
            DestR.h = textHeight;
            SDL_RenderCopy(renderer, gameState->youDiedText.texture, NULL, &DestR);
        }
    }

    //**********************FRAME******************
    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = FRAME_WIDTH;
    SrcR.h = FRAME_HEIGHT;
    DestR.x = scalingInfo.offsetX;
    DestR.y = scalingInfo.offsetY;
    DestR.w = FRAME_WIDTH * scalingInfo.scalingFactor;
    DestR.h = FRAME_HEIGHT * scalingInfo.scalingFactor;
    SDL_RenderCopy(renderer, gameState->frame_texture, &SrcR, &DestR);

    if (gameState->paused)
        draw_paused_bar(renderer, gameState->font, &scalingInfo, gameState->selectedOptionPaused.value);
    else
        draw_bar(renderer, &(gameState->player), gameState->bar_texture, &scalingInfo);

    //*************************************RECTANGLES TO HIDE SPRITES OUT OF BOUNDS
    DestR.x = 0;
    DestR.y = 0;
    DestR.w = scalingInfo.rendererWidth;
    DestR.h = scalingInfo.offsetY + 1;
    
    SDL_SetRenderDrawColor(renderer, 64, 0, 25, 255);

    SDL_RenderFillRect(renderer, &DestR);//top
    DestR.y = scalingInfo.offsetY + (FRAME_HEIGHT * scalingInfo.scalingFactor);
    
    SDL_RenderFillRect(renderer, &DestR);//bottom

    DestR.w = scalingInfo.offsetX;
    DestR.y = scalingInfo.offsetY;
    DestR.h = FRAME_HEIGHT * scalingInfo.scalingFactor;
    SDL_RenderFillRect(renderer, &DestR);//left
}

void gameState_update(gameState_t *gameState, float delta)
{
    if (gameState->paused)
    {
        update_keys(&(gameState->keys)); // call after done using keys in the iteration
        return;
    }

    if (!gameState->player.isDead)
    {
        update_player(&gameState->player, &gameState->keys, delta);

        player_map_collision(&gameState->player, &gameState->map);

        update_enemies(&(gameState->enemies), &(gameState->enemy_projectiles), &(gameState->player), delta);

        player_attack_enemies(&(gameState->enemies), &(gameState->player), &(gameState->map));

        update_projectiles(&(gameState->enemy_projectiles), delta);

        enemies_map_collision(&(gameState->enemies), &gameState->map);

        projectiles_map_collision(&(gameState->enemy_projectiles), &(gameState->map));

        player_enemy_projectiles_collision(&(gameState->player), &(gameState->enemy_projectiles));

        cast_rays(&gameState->player, gameState->rays, &gameState->map);
    }
    else
    {
        if (!gameState->deathAnimFinished)
        {
            fizzlefade(gameState, delta); 
        }
        else
        { // death anim finished
            if (gameState->keys.anyKeyPressed)
            {
                gameState->quit = true;
                return;
            }
        }
    }

    update_keys(&(gameState->keys)); // call after done using keys in the iteration
}

void update_enemies(enemy_arr_t *enemies, projectile_arr_t *enemy_projectiles, player_t *player, float delta)
{
    for (int i = 0; i < enemies->size; i++)
    {
        enemy_t *e = &enemies->enemies[i];

        // MOVE ENEMY
        vec2f dirToPlayer = vector_sub(player->pos, e->pos);
        dirToPlayer = vector_norm(dirToPlayer);
        const float speed = 0.75f;
        e->pos = vector_add(e->pos, vector_mult_scalar(dirToPlayer, delta * speed));
        e->circle.p.x = e->pos.x;
        e->circle.p.y = e->pos.y;
        //

        // SHOOT ENEMY
        e->fAttackCounter += delta;
        if (e->fAttackCounter >= ENEMY_SHOOT_DELAY)
        {
            projectile_t p;
            p.pos = e->pos;
            p.vDir = vector_norm(vector_sub(player->pos, e->pos));
            p.fLifetime = 0.0f;
            p.circle.p.x = p.pos.x;
            p.circle.p.y = p.pos.y;
            p.circle.r = 0.1f;
            add_procetile(enemy_projectiles, p);
            e->fAttackCounter = 0.0f;
        } //

        // ANIMATE ENEMY
        if (e->hitAnimPlaying)
        {
            e->fHitAnimCounter += delta;
            if (e->fHitAnimCounter >= ENEMY_HIT_ANIM_TIME)
                e->hitAnimPlaying = false;
        }
        //
    }
}

void player_attack_enemies(enemy_arr_t *enemies, player_t *player, map_t *map)
{
    if (player->bJustAttacked)
    {
        switch (player->selectedWeapon)
        {
        case WEAPON_HANDGUN:
            player_shoot_enemies_raycast(enemies, player, map, 420.0f);
            break;
        case WEAPON_KNIFE:
            player_shoot_enemies_raycast(enemies, player, map, KNIFE_REACH);
            break;
        case WEAPON_LAUNCHER:
            // instantiate projectile
            break;
        }
    }
}

void player_shoot_enemies_raycast(enemy_arr_t *enemies, player_t *player, map_t *map, float maxDistance)
{
    ray_t ray = castSinleRay(player->pos, player->dir, map);

    for (int i = 0; i < enemies->size; i++)
    {
        enemy_t *e = &enemies->enemies[i];
        float dist = rayPointDistSquared(player->pos, player->dir, e->pos);
        if (dist <= e->width * 0.5f * e->width * 0.5f)
        {
            float squareDistToEnemy = distanceSquared(e->pos.x, e->pos.y, player->pos.x, player->pos.y);
            if (ray.bHit)
            {
                if (ray.fCorrectedDistance * ray.fCorrectedDistance < squareDistToEnemy)
                    continue; // wall hit is closer than enemy
            }
            if (squareDistToEnemy > maxDistance * maxDistance)
                continue; // enemy is too far, out of weapons reach

            vec2f vToEnemy = vector_sub(e->pos, player->pos);

            float dot = vector_dot(vToEnemy, player->dir);
            if (dot < 0.0f)
                continue; // enemy is behind player

            enemy_take_damage(e);
            // printf("hit");
        }
    }
}

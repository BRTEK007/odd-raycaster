#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define CUTE_C2_IMPLEMENTATION
#include "cute_c2.h"

#include "global.h"
#include "vec2f.h"
#include "input.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
#include "draw.h"
#include "map.h"
#include "rays.h"
#include "gameState.h"
#include "collision.h"
#include "menuState.h"


void draw_fps(SDL_Renderer *renderer, TTF_Font *font, unsigned fps)
{
    char buffer[16];
    sprintf(buffer, "%04d", fps);
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, buffer, (SDL_Color){255, 255, 0});

    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect destR;
    destR.x = 0;
    destR.y = 0;
    destR.w = surfaceMessage->w;
    destR.h = surfaceMessage->h;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &destR);

    SDL_RenderCopy(renderer, Message, NULL, &destR);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

int main(int argc, char *argv[])
{
#ifdef THREADED_FLOOR_RENDERING
    printf("-- using multithreding, threads: %d\n", THREAD_COUNT);
#endif

    (void)argc;
    (void)argv;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(1);

    if (TTF_Init() < 0)
        exit(1);

    SDL_Window *window = SDL_CreateWindow("raycaster",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          FRAME_WIDTH * 3 + 20, FRAME_HEIGHT * 3 + 20,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
        exit(1);

    SDL_Renderer *renderer;

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        exit(1);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    bool quit = false;
    gameState_t gameState;
    menuState_t menuState;
    state_t state = MENU_STATE;
    long long last = current_timestamp();
    float fTimeCounter = 0.0f;
    int iFrameCounter = 0;
    int fps = 0;
    TTF_Font *font = TTF_OpenFont("../assets/font/pixel-ultima-regular.ttf", 42);
    if (font == NULL)
        exit(1);

    // gameState_init(&gameState, renderer, font);
    menuState_init(&menuState, renderer, font);
    // Event loop


    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }

            switch (state)
            {
            case MENU_STATE:
                menuState_event(&menuState, event);
                break;
            case GAME_STATE:
                gameState_event(&gameState, event);
                break;
            }
        }

        long long now = current_timestamp();
        double delta = (double)(now - last) / 1000.f;

        switch (state)
        {
        case MENU_STATE:
            menuState_update(&menuState, delta);
            menuState_draw(&menuState, renderer);
            break;
        case GAME_STATE:
            gameState_update(&gameState, delta);
            gameState_draw(&gameState, renderer);
            break;
        }

        switch (state)
        {
        case MENU_STATE:
            if (menuState.quit)
            {
                if (menuState.selectedOption.value == 1) // MENU -> EXIT APP
                {
                    quit = true;
                }
                else if (menuState.selectedOption.value == 0) // MENU -> GAME
                {
                    state = GAME_STATE;
                    gameState_init(&gameState, renderer, font);
                    menuState_free(&menuState);
                }
            }
            break;
        case GAME_STATE:
            if (gameState.quit) // GAME -> MENU
            {
                state = MENU_STATE;
                gameState_free(&gameState);
                menuState_init(&menuState, renderer, font);
            }
            break;
        }

        fTimeCounter += delta;
        iFrameCounter++;

        if (fTimeCounter > 0.1f)
        {
            fps = (unsigned)floor(iFrameCounter / fTimeCounter);
            fTimeCounter = 0.0f;
            iFrameCounter = 0;
        }

        last = now;

        draw_fps(renderer, font, fps);
        SDL_RenderPresent(renderer);
    }

    switch (state)
    {
    case MENU_STATE:
        menuState_free(&menuState);
        break;
    case GAME_STATE:
        gameState_free(&gameState);
        break;
    }

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    TTF_Quit();

    exit(0);
}
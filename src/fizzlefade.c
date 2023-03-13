#include "fizzlefade.h"
#include "SDL2/SDL.h"
#include "gameState.h"

void fizzlefade(GameState* gameState, float delta){
    Uint32 color = SDL_MapRGBA(gameState->deathScreenSurface->format, 0, 0, 0, 255);

    unsigned pixels_fizzeled = 0;
    unsigned pixels_to_fizzle = delta * 1000 * 100;//fizzle at speed of 100 pixels per second

    while (pixels_fizzeled < pixels_to_fizzle)
    {
        uint16_t y = gameState->rndval / SCREEN_WIDTH;
        uint16_t x = gameState->rndval % SCREEN_WIDTH;
        
        unsigned lsb = gameState->rndval & 1;            
        gameState->rndval >>= 1;                       
        
        if (lsb)
            gameState->rndval ^= 0x00012000;
        
        if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        {
            setPixelFast(gameState->deathScreenSurface, x, y, color);
            pixels_fizzeled++;
        }
        if (gameState->rndval == 1)//all pixels painted
        {
            gameState->isDeathAnimFinished = true;
            break;
        }
    }

}
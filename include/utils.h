#ifndef UTILS_H
#define UTILS_H
#include "SDL2/SDL.h"
#include <sys/time.h>

#define PI 3.142857

typedef struct intInRange_t
{ // keeps value between min and max, no cycling
    int value, min, max;
} intInRange_t;

long long current_timestamp();
void set_pixel_fast(SDL_Surface *, int, int, Uint32);
Uint32 get_pixel_fast(SDL_Surface *, int, int);
float distanceSquared(float x1, float y1, float x2, float y2);

intInRange_t intInRange_create(int v, int min, int max);
void intInRange_increment(intInRange_t *iir);
void intInRange_decrement(intInRange_t *iir);

int clampInt(int v, int min, int max);

#endif // UTILS_H

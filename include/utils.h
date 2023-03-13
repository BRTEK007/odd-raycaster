#ifndef UTILS_H
#define UTILS_H
#include "SDL2/SDL.h"
#include <sys/time.h>

#define PI 3.142857

typedef struct IntInRange
{ // keeps value between min and max, no cycling
    int value, min, max;
} IntInRange;

long long currentTimestamp();
void setPixelFast(SDL_Surface *, int, int, Uint32);
Uint32 getPixelFast(SDL_Surface *, int, int);
float distanceSquared(float x1, float y1, float x2, float y2);

IntInRange IntInRange_create(int v, int min, int max);
void IntInRange_increment(IntInRange *iir);
void IntInRange_decrement(IntInRange *iir);

int clampInt(int v, int min, int max);

#endif // UTILS_H

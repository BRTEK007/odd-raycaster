#include "utils.h"

long long currentTimestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);                                         
    long long ms = tv.tv_sec * 1000LL + tv.tv_usec / 1000; 
    return ms;
}

void setPixelFast(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 *const target_pixel = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

Uint32 getPixelFast(SDL_Surface *surface, int x, int y)
{
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
    return *(Uint32 *)p;
}

float distanceSquared(float x1, float y1, float x2, float y2)
{
    return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

IntInRange IntInRange_create(int v, int min, int max)
{
    IntInRange iir;
    iir.value = v;
    iir.min = min;
    iir.max = max;
    return iir;
}
void IntInRange_increment(IntInRange *iir)
{
    iir->value++;
    if (iir->value > iir->max)
        iir->value = iir->max;
}

void IntInRange_decrement(IntInRange *iir)
{
    iir->value--;
    if (iir->value < iir->min)
        iir->value = iir->min;
}

int clampInt(int v, int min, int max)
{
    if (v < min)
        return min;
    if (v > max)
        return max;
    return v;
}
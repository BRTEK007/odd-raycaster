#include "vec2f.h"
#include <math.h>

vec2f vector_add(vec2f a, vec2f b)
{
    vec2f c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

vec2f vector_sub(vec2f a, vec2f b)
{
    vec2f c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

vec2f vector_mult_scalar(vec2f a, float s)
{
    a.x *= s;
    a.y *= s;
    return a;
}

float vector_mag(vec2f v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

float vector_dot(vec2f a, vec2f b){
    return a.x*b.x + a.y*b.y;
}

vec2f vector_norm(vec2f v)
{
    float m = vector_mag(v);
    if(m > 0)
    {
        v.x = v.x/m;
        v.y = v.y/m;
    }
    else
    {
        v.x = 0;
        v.y = 0;
    }
    return v;
}

vec2f vector_right(vec2f v){
    vec2f v2;
    v2.x = -v.y;
    v2.y = v.x;
    return v2;
}

#ifndef VEC2F_H
#define VEC2F_H

typedef struct vec2f
{
    float x, y;
} vec2f;

vec2f vector_add(vec2f, vec2f);

vec2f vector_sub(vec2f, vec2f);

vec2f vector_mult_scalar(vec2f, float);

float vector_mag(vec2f);

vec2f vector_norm(vec2f);

vec2f vector_right(vec2f);

float vector_dot(vec2f a, vec2f b);

#endif // VEC2F_H

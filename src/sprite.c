#include "sprite.h"

void sprite_arr_init(sprite_arr_t *a, int s)
{
    a->size = 0;
    a->capacity = s;
    a->arr = calloc(s, sizeof(sprite_t));
    if (a->arr == NULL)
        exit(1);
}

void sprite_arr_free(sprite_arr_t *a)
{
    free(a->arr);
}

void sprite_arr_add(sprite_arr_t *a, sprite_t s)
{
    if (a->size == a->capacity)
        return;
    a->arr[a->size] = s;
    a->size++;
}
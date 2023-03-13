#include "sprite.h"

void SpriteArray_init(SpriteArray *a, int s)
{
    a->size = 0;
    a->capacity = s;
    a->arr = calloc(s, sizeof(Sprite));
    if (a->arr == NULL)
        exit(1);
}

void SpriteArray_free(SpriteArray *a)
{
    free(a->arr);
}

void SpriteArray_add(SpriteArray *a, Sprite s)
{
    if (a->size == a->capacity)
        return;
    a->arr[a->size] = s;
    a->size++;
}
#include "projectile.h"

void ProjectileArray_init(ProjectileArray *arr, int s)
{
    arr->size = 0;
    arr->capacity = s;
    arr->arr = calloc(s, sizeof(Projectile));
    if (arr->arr == NULL)
        exit(1);
}

void ProjectileArray_free(ProjectileArray *arr)
{
    free(arr->arr);
}

void ProjectileArray_add(ProjectileArray *arr, Projectile p)
{
    if (arr->size == arr->capacity)
        return;
    arr->arr[arr->size] = p;
    arr->size++;
}

void ProjectileArray_remove(ProjectileArray *arr, int index)
{
    if (index < 0 || index >= arr->size)
        return;

    for (int i = index; i < arr->size - 1; i++)
    {
        arr->arr[i] = arr->arr[i + 1];
    }
    arr->size--;
}

Sprite Projectile_getSprite(Projectile *e)
{
    Sprite s;
    s.pos = e->pos;
    s.tx = 0;
    s.ty = 32 + 1;
    s.tw = 32;
    s.th = 32;
    return s;
}
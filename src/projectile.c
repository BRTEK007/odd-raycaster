#include "projectile.h"

void init_procetile_arr(projectile_arr_t *arr, int s)
{
    arr->size = 0;
    arr->capacity = s;
    arr->arr = calloc(s, sizeof(projectile_t));
    if (arr->arr == NULL)
        exit(1);
}

void free_procetile_arr(projectile_arr_t *arr)
{
    free(arr->arr);
}

void add_procetile(projectile_arr_t *arr, projectile_t p)
{
    if (arr->size == arr->capacity)
        return;
    arr->arr[arr->size] = p;
    arr->size++;
}

void projectile_arr_remove(projectile_arr_t *arr, int index)
{
    if (index < 0 || index >= arr->size)
        return;

    for (int i = index; i < arr->size - 1; i++)
    {
        arr->arr[i] = arr->arr[i + 1];
    }
    arr->size--;
}

sprite_t projectile_get_sprite(projectile_t *e)
{
    sprite_t s;
    s.pos = e->pos;
    s.tx = 0;
    s.ty = 32 + 1;
    s.tw = 32;
    s.th = 32;
    return s;
}
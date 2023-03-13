#include "map.h"

void init_map(map_t *map)
{
    const char MAP[16][16] =
        {
            "EEEEEEEEEEEEEEEE",
            "E#########C#E##E",
            "E##########CE##E",
            "E##D##C########E",
            "E##############E",
            "E##############E",
            "E##B##A########E",
            "E##########AE##E",
            "E#########A#E##E",
            "E##############E",
            "E##############E",
            "E##############E",
            "E##############E",
            "E##############E",
            "E##############E",
            "EEEEEEEEEEEEEEEE"};
    map->height = 16;
    map->width = 16;
    map->arr = calloc(map->height, sizeof(map_element_t *));

    for (int i = 0; i < map->width; i++)
        map->arr[i] = calloc(map->height, sizeof(map_element_t));

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            map->arr[y][x].blockType = MAP[y][x];
        }
    }
}

void free_map(map_t *map)
{
    for (int i = 0; i < map->height; i++)
    {
        free(map->arr[i]);
    }
    free(map->arr);
}
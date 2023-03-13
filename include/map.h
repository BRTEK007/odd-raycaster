#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

typedef struct map_element_t{
    char blockType;
    char textures[4];
} map_element_t;

typedef struct map_t{
    int width;
    int height;
    map_element_t** arr;
} map_t;

void init_map(map_t* map);
void free_map(map_t* map);

#endif
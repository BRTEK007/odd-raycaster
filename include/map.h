#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

typedef struct MapCell{
    char blockType;
    char textures[4];
} MapCell;

typedef struct Map{
    int width;
    int height;
    MapCell** arr;
} Map;

void Map_init(Map* map);
void Map_free(Map* map);

#endif
#include "map.h"

#include <stdlib.h>

Map *Map_create(int width, int height)
{
    Map *map = malloc(sizeof(Map));
    map->width = width;
    map->height = height;

    map->cells = malloc(sizeof(Cell)*map->width*map->height);

    int y, x;
    for (y = 0; y < map->height; y++)
    {
        for (x = 0; x < map->width; x++)
        {
            unsigned int a = random();
            (Map_getCell(map, x, y))->a = a;
            (Map_getCell(map, x, y))->b = 0;
        }
    }

    return map;
}

Cell *Map_getCell(Map *map, int x, int y)
{
    if (x < 0 || x >= map->width)
        return NULL;
    if (y < 0 || y >= map->height)
        return NULL;

    return (map->cells + y*map->width + x);
}

void Map_destroy(Map *map)
{
    free(map->cells);
    free(map);
}

void Map_operate(Map *map)
{
    int i, j;
    Cell cell;
    for (j = 0; j < map->height; j++)
    {
        for (i = 0; i < map->width; i++)
        {
            Cell *c = Map_getCell(map, i, j);
            cell = *c;
            *(map->cells) = cell;
        }
    }
}



#ifndef  MAP_INC
#define  MAP_INC

#include "data.h"

struct Map {
    unsigned int width;
    unsigned int height;
    Cell *cells;
};				/* ----------  end of struct Map  ---------- */

typedef struct Map Map;

Map *Map_create(int width, int height);
Cell *Map_getCell(Map *map, int x, int y);
void Map_destroy(Map *map);

#endif   /* ----- #ifndef MAP_INC  ----- */

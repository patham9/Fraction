#ifndef  PART_INC
#define  PART_INC

#include "pool.h"
#include "map.h"

#include "lzo/lzo1x.h"

struct Part {
    int x;
    int y;
    unsigned int width;
    unsigned int height;
    Cell *cells;
};				/* ----------  end of struct Part  ---------- */

typedef struct Part Part;

struct PartInfo {
    int x;
    int y;
    unsigned int width;
    unsigned int height;
};
typedef struct PartInfo PartInfo;

struct CompressedPart {
    char *data;
    unsigned int size;
    unsigned int src_size;
};
typedef struct CompressedPart CompressedPart;

Part *Part_create(Pool *part_pool, int x, int y, 
                  int width, int height);
void Part_operate(Pool *pool, Part *part);
Cell *Part_getCell(Part *part, int x, int y);

CompressedPart Part_compress(Pool *part_pool, Part *part);


#endif   /* ----- #ifndef PART_INC  ----- */

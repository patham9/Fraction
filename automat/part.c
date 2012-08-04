#include "part.h"

void *Part_initialize(Part *part, Map *map)
{
    int i, j;
    for (j = 0; j < part->height; j++)
    {
        memcpy(part->cells + j*part->height, 
               Map_getCell(map, part->x, part->y + j), 
               sizeof(Cell)*part->width);
    }
 
}

Part *Part_create(Pool *part_pool, int x, int y, int width, int height)
{
    Part *part = p_alloc(part_pool, sizeof(Part));
    part->x = x;
    part->y = y;
    part->width = width;
    part->height = height;
    part->cells = (Cell *)p_alloc(part_pool, sizeof(Cell)*part->height*part->width);
   
    return part;
}

Cell *Part_getCell(Part *part, int x, int y)
{
    return (part->cells + y*part->width + x);
}

void Part_operate(Pool *pool, Part *part)
{
    int i, j;
    for (j = 0; j < part->height; j++)
    {
        for (i = 0; i < part->width; i++)
        {
            Cell *c = Part_getCell(part, i, j);
        }
    }
}

CompressedPart Part_compress(Pool *part_pool, Part *part)
{
    lzo_init();
    void *wrkmem = p_alloc(part_pool, LZO1X_MEM_COMPRESS); // 64 KByte?
    if (wrkmem == NULL)
    {
        CompressedPart cp;
        cp.data = NULL;
        cp.size = 0;
        return cp;
    }

    unsigned int data_size = sizeof(PartInfo) + sizeof(Cell)*part->width*part->height;
    char *dst = p_alloc(part_pool, data_size + data_size/64 + 1);
    if (dst == NULL)
    {
        CompressedPart cp;
        cp.data = NULL;
        cp.size = 0;
        return cp;
    }
    char *src = p_alloc(part_pool, data_size);
    if (src == NULL)
    {
        CompressedPart cp;
        cp.data = NULL;
        cp.size = 0;
        return cp;
    }
    PartInfo part_info;
    part_info.x = part->x;
    part_info.y = part->y;
    part_info.width = part->width;
    part_info.height = part->height;

    memcpy(src, &part_info, sizeof(PartInfo));
    memcpy(src + sizeof(PartInfo), part->cells, sizeof(Cell)*part->width*part->height);

    unsigned int compressed_size = 0;
    lzo1x_1_compress(src, data_size, dst, &compressed_size, wrkmem);

    CompressedPart cp;
    cp.data = dst;
    cp.size = compressed_size;
    cp.src_size = data_size;
    return cp;
}

Part *Part_decompress(Pool *part_pool, CompressedPart compressed_part)
{
    char *dst = p_alloc(part_pool, compressed_part.src_size);
    unsigned int src_size = 0;
    lzo1x_decompress(compressed_part.data, compressed_part.size, dst, &src_size, NULL);

    PartInfo *p = dst;

    Part *part = Part_create(part_pool, p->x, p->y, p->width, p->height);
    part->cells = dst + sizeof(PartInfo);
    return part;
}


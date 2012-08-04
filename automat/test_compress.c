#include <stdlib.h>

#include "data.h"
#include "part.h"
#include "map.h"
#include "pool.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main (int argc, char *argv[])
{
    srand(time(0));
    Map *map = Map_create(512, 512);

    Pool *pool = p_initialize(1024*(64 + 4 + 1024)); // 64 KB

    Part *part = Part_create(pool, 0, 0, 32, 32);
    Part_initialize(part, map);

//    Map_operate(map);
    int j, i;
    int sum = 0;
    for (j = 0; j < 32; j++)
    {
        for (i = 0; i < 32; i++)
        {
            sum += Part_getCell(part, i, j)->a;
            sum += Part_getCell(part, i, j)->b;
        }
    }
    printf("Datasum: %d\n", sum);
    CompressedPart cp = Part_compress(pool, part);
    if (cp.data == NULL)
    {
        p_free(pool);
        return -1;
    }

    Pool *dec_pool = p_initialize(1024*9);
    Part *dec_part = Part_decompress(dec_pool, cp);
    sum = 0;
    for (j = 0; j < 32; j++)
    {
        for (i = 0; i < 32; i++)
        {
            sum += Part_getCell(dec_part, i, j)->a;
            sum += Part_getCell(dec_part, i, j)->b;
        }
    }

    printf("Datasum: %d\n", sum);

    printf("Compressed size: %d\n", cp.size);

    p_free(dec_pool);
    p_free(pool);

    Map_destroy(map);
    map = NULL;
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

#include "pool.h"

Pool *p_initialize(unsigned int size)
{
    Pool *p = malloc(sizeof(Pool));
    p->size = size;
    p->begin = NULL;
    p->end = NULL;
}

void *p_alloc(Pool *pool, unsigned int size)
{
    if (pool->size < size)
    {
        return NULL;
    }

    if (pool->begin == NULL)
    {
        pool->begin = malloc(pool->size);
        pool->end = pool->begin;
    }

    void *ptr = pool->end;
    pool->end = ptr + size;
    pool->size -= size;

    return ptr;
}

void *p_free(Pool *pool)
{
    free(pool->begin);
    free(pool);
}

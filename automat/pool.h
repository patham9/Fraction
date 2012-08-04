#ifndef  POOL_H_INC
#define  POOL_H_INC

#define NULL 0

struct Pool {
    unsigned int size;
    void *begin;
    void *end;
};				/* ----------  end of struct Pool  ---------- */

typedef struct Pool Pool;

Pool *p_initialize(unsigned int size);
void *p_alloc(Pool *pool, unsigned int size);
void *p_free(Pool *pool);

#endif   /* ----- #ifndef POOL_H_INC  ----- */


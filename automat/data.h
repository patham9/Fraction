#ifndef  DATA_H
#define	DATA_H			/*  */

#include "pool.h"

struct Cell {
    int a:12;
    int b:20;
};				/* ----------  end of struct Cell  ---------- */

typedef struct Cell Cell;

#endif     /* -----  not DATA_H  ----- */


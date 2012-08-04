#ifndef Automat_H
#define Automat_H

#include "Hamlib/Hauto.h"
#include "Cell.h"

void Automat_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells);
#define Def(name,condition) float name(Cell *c,Cell *ref){ return condition; }

#endif

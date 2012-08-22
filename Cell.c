#include "Cell.h"

Cell *Cell_NEW(int i,int j) 		 									//constructor for a new cell called for every cell when the automat constructor is called
{
	Cell *ret=(Cell*)calloc(1,sizeof(Cell));
	ret->i=i;
	ret->j=j;
	ret->state=GRASS;
	ret->rootwater=NULL;
	ret->height=landscape[i][j]*10.0;
	ret->house_distance=100000;  
	ret->forest_distance=100000; 
	return ret;
}

float being_a(Cell *c,int state)
{
	return c->state==state;
}

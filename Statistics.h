#ifndef Statistics_H
#define Statistics_H

#include "Game.h"
#include "Hamlib/Hauto.h"

typedef struct
{
	float amount_of_people;												//no type conversions, statistics uses always float, int doesnt make sense 
}Statistics;

Statistics* statistics_new();
void statistics_next(Hauto_OBJ* automat,Statistics* s);

#endif

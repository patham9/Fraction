#include "Statistics.h"
#include "Cell.h"

Statistics* statistics_new()
{
	return (Statistics*)calloc(1,sizeof(Statistics));
}
void statistics_next(Hauto_OBJ* automat,Statistics* s)
{
	int i,j;
	s->amount_of_people=0;
	s->amount_of_houses=0;
	for(i=1;i<automat->n-1;i++)
	{
		for(j=1;j<automat->n-1;j++)
		{
			if(GetCell(i,j,Cell,person))
				s->amount_of_people+=1;
			if(GetCell(i,j,Cell,state)==HOUSE)
				s->amount_of_houses+=1;
		}
	}
}

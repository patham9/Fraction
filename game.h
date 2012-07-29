typedef struct
{
	int lastchange;														//when did the cell change the last time? procedural texture generator wants to know
	int state;															//cell type
	float wateramount;													//ground water
	void *rootwater;													//root pointer (from which water cell does the water cell come from)
	float height;														//the height of the cell, at first resulting from the landscape, but can change then
	int wood;															//city resource
}Cell;

Cell *Cell_NEW(int i,int j) 		 									//constructor for a new cell called for every cell when the automat constructor is called
{
	Cell *ret=(Cell*)malloc(sizeof(Cell));								//allocating memory for the Cell pointer
	ret->state=GRASS;													//setting default state to GRASS
	ret->wateramount=0;													//no ground water by default
	ret->lastchange=0;													//
	ret->rootwater=NULL;												//no root pointer
	ret->height=landscape[i][j]*10.0;									//setting height to the landscape height at current position
	ret->wood=0;
	return ret;															//return our created object
}

typedef struct
{
	float x;
	float y;
	float targetx;
	float targety;
	int dead;
	int workstate;
	int wood;
}Men;

int maxmen=1000;
Men **men;
int meni=0;

void Men_Add(float x,float y,Cell ***readcells)
{
	int i;
	for(i=0;i<meni;i++)
	{
		if(men[i]==NULL || men[i]->dead==1)
		{
			break;
		}
		if(i==maxmen-1) //reached the maximum, no add
		{
			return;
		}
	}
	if(men[i]==NULL)
	{
		men[i]=(Men*)malloc(sizeof(Men));
	}
	men[i]->x=x;
	men[i]->y=y;
	men[i]->dead=0;
	men[i]->workstate=0;
	men[i]->wood=0;
	meni=min(maxmen,meni+1);
}

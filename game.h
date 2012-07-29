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
	if(meni>=maxmen){return;}
	men[meni]=(Men*)malloc(sizeof(Men));
	men[meni]->x=x;
	men[meni]->y=y;
	men[meni]->dead=0;
	men[meni]->workstate=0;
	men[meni]->wood=0;
	meni++;
}

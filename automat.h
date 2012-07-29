float being_a(Cell *c,int state)										//a function returning true if the state of cell c equals state else false
{
	return c->state==state;
}
void Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	float water_amount(Cell *c,void *z)									
	{
		return c->wateramount;											//a function returning the ground water amount of a cell
	}
	float water_and_higher_than(Cell *c,float *height)					
	{
		return c->state==WATER && c->height>*height;					//a function which only returns true if the cell c is of type water, and higher than height
	}
	writeme->lastchange=readme->lastchange+1;
	writeme->state=readme->state;										//eliminating not defined behavior of not handled cases
	if(readme->state==WATER)																			
	{
		writeme->wateramount=1;											//water delivers maximum ground water to its surrounding
	}
	if(readme->state!=WATER)											//ground water also gets distributed to its neighborhood
	{
		writeme->wateramount=9*NeighborsValue(op_max,water_amount,NULL)/10; 
	} 
	if(t%100==0 && readme->state==GRASS && NeighborsValue(op_plus,being_a,FOREST)==3 && NeighborsValue(op_plus,water_amount,NULL)/N>0.1) //also try 2 instead 3
	{
		writeme->state=FOREST;											//a cell which is grass and has 3 wood neighbours and enough ground water becomes forest
		writeme->lastchange=0;
	}
	if(readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,water_and_higher_than,&readme->height))
	{ 
		writeme->state=WATER; 											//(water flows downwards) a cell which isn't rock which has neighbors with water on a higher position gets water and remembers a root cell
		writeme->rootwater=FirstNeighbor(water_and_higher_than,&readme->height);
		writeme->lastchange=0;
	}
	if(readme->state==WATER && readme->rootwater!=NULL && ((Cell*)readme->rootwater)->state!=WATER)
	{
		writeme->state=GRASS;											//water without having a root water has lost its source
		writeme->rootwater=NULL; 
		writeme->lastchange=0;
	}
	if(readme->state==GRASS)											//grass with a city near gets city if neighbour citys have enough wood
	{
		int buildingCost=200;
		float wood(Cell *c,void *z)
		{
			return c->wood;
		}
		float same_amount_of_wood(Cell *c,float wood)
		{
			return c->wood==wood;
		}
		float maxWood=NeighborsValue(op_max,wood,NULL);
		float waterAround=NeighborsValue(op_plus,being_a,WATER);
		if(maxWood>=buildingCost && t%100==0)
		{
			float value=frnd();
			if(value>0.5 && waterAround==0)
			{
				writeme->state=CITY;
			}
			else
			if(value<0.13 || waterAround>0)
			{
				writeme->state=ROCK;
			}
			else
			{
				writeme->state=STREET;
			}
		}
	}
	if(t%100==0 && readme->state==CITY) //it can happen than a men gets born here
	{
		Men_Add(i,j,readcells);
	}
}

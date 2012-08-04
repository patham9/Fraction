#include "Automat.h"
#include "Game.h"

Def( forest_distance                           , c->forest_distance                 )
Def( forest_distance_equals                    , c->forest_distance==*((float*)ref) )
float test=0;
Def( has_person_and_higher_forest_distance_than , c->person && c->forest_distance>test  ) //*((float*)ref)
void Pathfinding_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
    /////////// AN OBSTACLE DELETES PATH INFO AND PERSONS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==ROCK || readme->state==WATER)
	{
		writeme->house_distance=worldsize;
		writeme->forest_distance=worldsize;
		writeme->person=0;
	}
	/////////// A WOOD SENDS WOOD PATH INFORMATION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state!=FOREST && readme->state!=ROCK && readme->state!=WATER && NeighborsValue(op_plus,being_a,FOREST)>=1)
	{
		writeme->forest_distance=1;
	}
	else
	if(readme->state!=FOREST && readme->state!=WATER && readme->state!=ROCK)
	{ 
		float dist=NeighborsValue(op_min,forest_distance,NULL);
		writeme->forest_distance=dist+1;
	}
	/////////// A PERSON WANTS TO GET WOOD SO THE PERSON GOES THE WAY OF SHORTEST forest_distance //////////////////////////////////////////////////////////////////////////////////////////
	test=readme->forest_distance;
	if(readme->person==0 && readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,has_person_and_higher_forest_distance_than,readme))
	{
		Cell* first=FirstNeighbor(has_person_and_higher_forest_distance_than,readme);
		writeme->person=1;
		writeme->job=first->job;
		SetCell(first->i,first->j,Cell,person,0);  //<-side effect be careful, but we don't want clones.
	}
}
void Population_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// A HOUSE HAS A POSSIBILITY FOR SPAWNING PEOPLE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==HOUSE && drnd()>0.99)
	{
		writeme->person=1;
	}
}
Def( water_amount          , c->wateramount                             )
Def( water_and_higher_than , c->state==WATER && c->height > ref->height )
void Vegetation_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// A CELL WHICH IS GRASS AND HAS 3 WOOD NEIGHBORS AND ENOUGH GROUND WATER BECOMES FOREST ////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==GRASS && NeighborsValue(op_plus,being_a,FOREST)==3 && NeighborsValue(op_plus,water_amount,NULL)/N>0.1)
	{
		writeme_state(FOREST);
	}
}
void Water_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// WATER DELIVERS MAXIMUM GROUND WATER TO ITS SURROUNDING ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==WATER)
	{
		writeme->wateramount=1;
	}
	/////////// GROUND WATER ALSO GETS DISTRUBUTED TO ITS NEIGHBORHOOD	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state!=WATER)
	{
		writeme->wateramount=9*NeighborsValue(op_max,water_amount,NULL)/10;
	}
	/////////// (WATER FLOWS DOWNWARDS) A CELL WHICH ISN'T ROCK WHICH HAS NEIGHBORS WITH WATER ON A HIGHER POSITION GETS WATER AND REMEMBERS A ROOT CELL /////////////////////////////////////
	if(readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,water_and_higher_than,readme))
	{ 
		writeme_state(WATER);
		writeme->rootwater=FirstNeighbor(water_and_higher_than,readme);
	} 
	/////////// WATER WITHOUT HAVING A ROOT WATER HAS LOST ITS SOURCE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==WATER && readme->rootwater!=NULL && ((Cell*)readme->rootwater)->state!=WATER)
	{
		writeme_state(GRASS);
		writeme->rootwater=NULL;
	}
}
void Weather_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells){}
void Automat_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	writeme->i=readme->i;												//never forget these
	writeme->j=readme->j;												//because Hauto allows change based automats too
	writeme->lastchange=readme->lastchange+1;							//some rules may function without these
	writeme->person=readme->person;										//but i don't want to debug automats and waste brain power about all the cases where this is needed and not :D
	writeme->job=readme->job;
	writeme->state=readme->state;
	writeme->height=readme->height; 
	writeme->wateramount=readme->wateramount;
	writeme->rootwater=readme->rootwater;
	writeme->house_distance=readme->house_distance;
	writeme->forest_distance=readme->forest_distance;
	writeme->wood=readme->wood;
	
	Water_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Vegetation_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Pathfinding_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Population_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Weather_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
}


#include "Automat.h"
#include "Game.h"
#include "Draw.h"

Def( forest_distance    , c->forest_distance 	  )
Def( house_distance     , c->house_distance  	  )
Def( being_a_person     , c->person!=not_a_person )
void Pathfinding_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	Distributes_Path_Information(FOREST, forest_distance);
	Person_Minimizes(rockfeller, forest_distance);
	Distributes_Path_Information(HOUSE , house_distance);
	Person_Minimizes(homecomer,  house_distance);  
}
void Person_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{ 
	/////////// A PERSON WHICH ALREADY HAS WOOD HAS TO BRING IT BACK////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->person==1 && NeighborsValue(op_plus,being_a,FOREST)>0)
	{
		writeme_person(homecomer);
	}
	/////////// A WOODFELLER WHO FOUND A TREE DESTROYS IT//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==FOREST && NeighborsValue(op_or,being_a_person,NULL))
	{
		writeme_state(GRASS); 
	}
	/////////// A PERSON BROUGHT WOOD BACK GOES INTO THE HOUSE AND BUILDS THIS CITY ON ROCK'N ROLL ///////////////////////////////////////////////////////////////////////////////////////////
	if(readme->person==homecomer && readme->state!=HOUSE && NeighborsValue(op_plus,being_a,HOUSE)>0)
	{
		writeme_person(not_a_person);
		if(NeighborsValue(op_plus,being_a,WATER)>0)
		{
			writeme->state=ROCK;
		}
		else
		{
			if(drnd()>0.8)
			{
				writeme_state(HOUSE);
			}
			else
			{
				writeme_state(STREET);
			}
		}
	}
}
void Population_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// A HOUSE HAS A POSSIBILITY FOR SPAWNING PEOPLE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==HOUSE && drnd()>0.99)
	{
		writeme_person(rockfeller);
	}
	/////////// DEATH BY ROCK OR WATER ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==ROCK || readme->state==WATER) //death by rock or water
	{
		writeme_person(not_a_person);
	}
}
Def( water_amount          , c->wateramount                             )
Def( water_and_higher_than , c->state==WATER && c->height > ref->height )
void Vegetation_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// A CELL WHICH IS GRASS AND HAS 3 WOOD NEIGHBORS AND ENOUGH GROUND WATER BECOMES FOREST ////////////////////////////////////////////////////////////////////////////////////////
	if(t%50==0 && readme->state==GRASS && NeighborsValue(op_plus,being_a,FOREST)==3 && NeighborsValue(op_plus,water_amount,NULL)/N>0.1)
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
	memcpy(writeme,readme,sizeof(Cell));
	writeme->lastchange++;
	Water_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Vegetation_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Pathfinding_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Person_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Population_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Weather_Simulate(t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
}


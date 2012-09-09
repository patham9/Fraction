#include "Automat.h"
#include "Game.h"
#include "Draw.h"

void Pathfinding_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	Distributes_Path_Information(state,FOREST,command_distance);
	Distributes_Path_Information(has_command,1,command_distance);
	Distributes_Path_Information(state,HOUSE,house_distance);
	Person_Minimizes(worker, command_distance);
	Person_Minimizes(homecomer,  house_distance);
}
Def( being_a_person     , c->person!=not_a_person   )
Def( being_a_worker , c->person==worker     		)  
Def( has_command     	, c->has_command            )
void Person_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{ 
	/////////// A ROCKFELLER WHO FINDS A TREE DESTROYS IT AND BRINGS IT HOME ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Interaction(person,worker,state,FOREST,triggers,writeme_person(homecomer),writeme_state(GRASS),1,0.5);
	/////////// A PERSON BROUGHT WOOD BACK BUILDS THIS CITY ON ROCK'N ROLL, FOR MORE INFO READ THIS: http://www.youtube.com/watch?v=DnUSGAi9j8I /////////////////////////////
	if(readme->person==homecomer && readme->state!=HOUSE && NeighborsValue(op_or,being_a,HOUSE))
	{
		writeme_person(worker);
		if(NeighborsValue(op_or,being_a,WATER))
			writeme->state=ROCK;
		else
		{
			if(drnd()>0.8 && stats->amount_of_houses<MAX_HOUSES)
				writeme_state(HOUSE);
			else
				writeme_state(STREET);
		}
	}
	/////////// A WORKER COMING TO A WORKING PLACE EXECUTES HIS JOB THERE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Interaction(has_command,1,person,worker,triggers,
	    if(readme->command==TERRAFORM_DOWN)
			{writeme->height-=0.1; writeme_state(GRASS);}
		else if(readme->command==TERRAFORM_UP)
			{writeme->height+=0.1; writeme_state(GRASS);}
		else if(readme->command!=HOUSE || stats->amount_of_houses<MAX_HOUSES)
			writeme_state(readme->command);
		writeme->has_command=0;
	,,1,1);
}
void Population_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// A HOUSE HAS A POSSIBILITY FOR SPAWNING PEOPLE, BUT ONLY TILL A MAX AMOUNT OF PEOPLE //////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==HOUSE && drnd()>0.9995 && stats->amount_of_people<MAX_PEOPLE)
		writeme_person(worker);
	/////////// DEATH BY ROCK OR WATER ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==ROCK || readme->state==WATER) //death by rock or water
		writeme_person(not_a_person);
}
Def( water_amount          , c->wateramount                             )
Def( water_and_higher_than , c->state==WATER && c->height > ref->height )
void Vegetation_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// A CELL WHICH IS GRASS AND HAS 3 WOOD NEIGHBORS AND ENOUGH GROUND WATER BECOMES FOREST ////////////////////////////////////////////////////////////////////////////////////////
	if(t%50==0 && readme->state==GRASS && NeighborsValue(op_plus,being_a,FOREST)==3 && NeighborsValue(op_plus,water_amount,NULL)/N>0.1)
		writeme_state(FOREST);
}
void Water_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// WATER DELIVERS MAXIMUM GROUND WATER TO ITS SURROUNDING ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==WATER)
		writeme->wateramount=1;
	/////////// GROUND WATER ALSO GETS DISTRUBUTED TO ITS NEIGHBORHOOD	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state!=WATER)
		writeme->wateramount=9*NeighborsValue(op_max,water_amount,NULL)/10;
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
Def( having_cloud          				 , c->cloud                             				   )
Def( not_water_and_higher_than           , c->state!=WATER && c->height > ref->height              )
void Weather_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	/////////// CLOUDS COME FROM EAST ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(i==automat->n-2 && frnd()>0.999)
	{
		writeme_cloud(1);
	}
	/////////// AND GO FROM EAST TO WEST /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->cloud)
	{
		writeme_cloud(0);
	}
	if(right->cloud)
	{
		if(frnd()<0.999)
			writeme_cloud(1);
		else
	////////// BUT SOMETIMES THE CLOUD GETS DESTROYED BECAUSE IT RAINS //////////////////////////////////////////////////////////////////////////////////////////////////
		if(frnd()<0.99)
		{
			writeme->wateramount=1;
		}
	////////// AND IN SOME RARE CASES SOMETHING REALLY BAD HAPPENS //////////////////////////////////////////////////////////////////////////////////////////////////////
		else
		{
			writeme->rootwater=FirstNeighbor(not_water_and_higher_than,readme);
            if(writeme->rootwater!=NULL)
            {
                writeme_state(WATER);
            }
		}
	}
	/////////// CLOUDS CAN GROW, AND FASTER IF THERE IS WATER UNDER IT ///////////////////////////////////////////////////////////////////////////////////////////////////
	if(readme->state==WATER && NeighborsValue(op_plus,having_cloud,NULL)>=3 && frnd()>0.9 || NeighborsValue(op_plus,having_cloud,NULL)>=1 && frnd()>0.99)
	{
		writeme_cloud(1);
	}
}
void Automat_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	memcpy(writeme,readme,sizeof(Cell));
	writeme->lastchange++;
	writeme->command_distance=10000;
	writeme->house_distance=10000;
	writeme->locked=0;
	Water_Simulate(stats,t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Vegetation_Simulate(stats,t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Pathfinding_Simulate(stats,t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Person_Simulate(stats,t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Population_Simulate(stats,t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
	Weather_Simulate(stats,t,i,j,writeme,readme,left,right,up,down,left_up,left_down,right_up,right_down,readcells);
}


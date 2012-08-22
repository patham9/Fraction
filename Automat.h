#ifndef Automat_H
#define Automat_H

#include "Hamlib/Hauto.h"
#include "Cell.h"

void Automat_Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells);

#define Def(name,condition) float name(Cell *c,Cell *ref){ return condition; }

#define Distributes_Path_Information(state_variable,distance_variable) 																	  	\
	if(readme->state!=state_variable && readme->state!=ROCK && readme->state!=WATER && NeighborsValue(op_plus,being_a,state_variable)>=1) 	\
	{ 																																      	\
		writeme->distance_variable=1;																										\
	}																																		\
	else 																																	\
	if(readme->state!=state_variable && readme->state!=WATER && readme->state!=ROCK)														\
	{ 																																		\
		writeme->distance_variable=NeighborsValue(op_min,distance_variable,NULL)+1;															\
	}																																		\
	if(readme->state==ROCK || readme->state==WATER || (readme->person!=not_a_person && readme->lastchange>0))								\
	{																																		\
		writeme->distance_variable=100000;																									\
	}
	//// ^ AN OBSTACLE DELETES PATH INFO PERSONS AND RESOURCES - PEOPLE STANDING AROUND ARE ALSO OBSTACLES BUT ONLY PATH INFO GETS REMOVED ///
	
#define Person_Minimizes(job,distance_variable)																															\
	Def( job##has_job_and_higher_goal_distance_than , c->person==job && c->distance_variable > readme->distance_variable ) 												\
	if(t%2==0 && readme->person==not_a_person && readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,job##has_job_and_higher_goal_distance_than,readme))\
	{																																									\
		Cell* first=FirstNeighbor(job##has_job_and_higher_goal_distance_than,readme);																					\
		writeme_person(job);																																			\
		SetCell(first->i,first->j,Cell,person,0);  																														\  
	}
	//^ side effect be careful, but we don't want clones.

#endif

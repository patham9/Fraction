#ifndef Automat_H
#define Automat_H

#include "Hamlib/Hauto.h"
#include "Cell.h"
#include "Statistics.h"

void Automat_Simulate(Statistics* stats,int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells);

#define Def(name,condition) float name(Cell *c,Cell *ref){ return condition; }

#define Distributes_Path_Information(variable,state_variable,distance_variable) 																	\
	Def( variable##state_variable##distance_variable##STATE    ,   c->variable==state_variable 	   )																		\
	Def( variable##state_variable##distance_variable##DISTANCE			 ,   c->distance_variable		  	   )																		\
	if(readme->variable!=state_variable && readme->state!=ROCK && readme->state!=WATER && NeighborsValue(op_plus,variable##state_variable##distance_variable##STATE,NULL)>=1)	\
	{ 																																      			\
		writeme->distance_variable=1;																												\
	}																																				\
	else 																																			\
	if(readme->variable!=state_variable && readme->state!=WATER && readme->state!=ROCK)																\
	{ 																																				\
	    if(readme->state==STREET)																													\
	      writeme->distance_variable=op_min(NeighborsValue(op_min,variable##state_variable##distance_variable##DISTANCE,NULL)+0.5,writeme->distance_variable);							\
		else 																																		\
		  writeme->distance_variable=op_min(NeighborsValue(op_min,variable##state_variable##distance_variable##DISTANCE,NULL)+1,writeme->distance_variable);							\
	}																																				\
	if(readme->state==ROCK || readme->state==WATER || (readme->person!=not_a_person && readme->lastchange>0))										\
	{																																				\
		writeme->distance_variable=100000;																											\
	}
	//// ^ AN OBSTACLE DELETES PATH INFO PERSONS AND RESOURCES - PEOPLE STANDING AROUND ARE ALSO OBSTACLES BUT ONLY PATH INFO GETS REMOVED //////////
	
#define Person_Minimizes(job,distance_variable)				\
	Def( job##has_job_and_higher_goal_distance_than , c->person==job && c->distance_variable > readme->distance_variable ) 												\
	if((t%2==0 || readme->state==STREET) && !readme->locked && readme->person==not_a_person && readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,job##has_job_and_higher_goal_distance_than,readme))\
	{																																									\
		Cell* first=FirstNeighbor(job##has_job_and_higher_goal_distance_than,readme);																					\
		writeme_person(job);																																			\
		readme->locked=1;																																		\
		SetCell(first->i,first->j,Cell,person,0);  																														\ 
		SetCell(first->i,first->j,Cell,locked,1);  																														\ 
	}
	//^ side effect be careful, but we don't want clones.
	
#define Interaction(variable1,value1,variable2,value2,triggers,event1,event2,possibility1,possibility2)									\
	Def( variable1##value1##variable2##value2    ,   c->variable1==value1)										\
	Def( variable2##value2##variable1##value1    ,   c->variable2==value2 )										\
	if(frnd()<=possibility1 && readme->variable1==value1 && NeighborsValue(op_or,variable2##value2##variable1##value1,NULL)){event1;}	\
	if(frnd()<=possibility2 && readme->variable2==value2 && NeighborsValue(op_or,variable1##value1##variable2##value2,NULL)){event2;}

#endif

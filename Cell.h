#ifndef Cell_H
#define Cell_H

#include "Game.h"

typedef struct
{ 
	//coordinates														
	int i;																
	int j;
	
	//timer
	int lastchange;														//since when is the cell what it is?
	
	//person:
	int person;
	int job;
	
	//landscape:
	int state;
	float height; 
	
	//water:
	float wateramount; 
	void *rootwater; 
	
	//path-finding:
	float house_distance;
	float forest_distance;
	
	//resources:
	int wood;
	
}Cell;
Cell *Cell_NEW(int i,int j); 		 									//constructor for a new cell called for every cell when the automat constructor is called

float being_a(Cell *c,int state);
#define writeme_state(value) {writeme->state=value; writeme->lastchange=0;}

#endif

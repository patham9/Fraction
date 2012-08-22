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
	enum {not_a_person,rockfeller,footman,homecomer} person;			
	
	//landscape:
	int state;
	float height; 
	
	//water:
	float wateramount; 
	void *rootwater; 
	
	//path-finding:
	float command_distance;
	float house_distance;
	float forest_distance;
	
	//player command:
	int command;
	int has_command;
	
}Cell;
Cell *Cell_NEW(int i,int j); 		 									//constructor for a new cell called for every cell when the automat constructor is called

float being_a(Cell *c,int state);
#define writeme_state(value) {writeme->state=value; writeme->lastchange=0; if(readme->state==WATER && writeme->state!=WATER || readme->state!=WATER && writeme->state==WATER){Draw_Set_HeightmapChanged();}}
#define writeme_person(value) {writeme->person=value; writeme->lastchange=0;}

#endif

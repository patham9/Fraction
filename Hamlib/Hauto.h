////////////Hamlib Module//////////////////////
//> Hauto - HCellularAutomat
//> A module for cellular automats
//+ Creator "Patrick Hammer"
//+ License "GPL" Language "C"
//+ Changedate "15.July.2012" State "80%"
///////////////////////////////////////////////
#ifndef Hauto
#define Hauto
///////////////////////////////////////////////

//---------------DEFINES---------------------//
#pragma region DEFINES
#define MODE 0 //0=speed 1=safeness/quality
#define EXT  1 //not only std
#pragma endregion
//-------------------------------------------//

//+++++++++++++++DEPENCIES+++++++++++++++++++//
#include "HamBuildControl.h"
#pragma region DEPENCIES
#include "Hutil.h"
#pragma endregion
//+++++++++++++++++++++++++++++++++++++++++++//

/////////////////MODULE-DATA///////////////////
struct Hauto
{
	int mode; //will be set to MODE on init 
	int init; //0
}
hauto;
typedef struct
{
	void (*Exec)(void*,int,int,int,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void ***readCells);
	void ***readCells;  //2D-array(**) of Cell objects(*)
	void ***writeCells; //2D-array(**) of Cell objects(*)
	int n;
	int t;
}Hauto_OBJ;
///////////////////////////////////////////////

//:::::::::::::::MODULE-IO::::::::::::::::::://
void hauto_INIT();
void hauto_UPDATE();
//::

Hauto_OBJ *Hauto_OBJ_NEW(int n,void (*Exec)(void* statistics,int t,int i,int j,void *writeme,void* readme,void* left,void* right,void* up,void* down,
                               void* left_up,void* left_down,void* right_up,void* right_down),void* (*New_Cell)(int,int));
void Hauto_OBJ_Exec(Hauto_OBJ *aut, void* statistics);
float Hauto_OBJ_NeighborsValue(float (*Op)(float, float),int i,int j,void ***readCells,float (*Neighbor_Value)(void*,void*),void* data);
float Hauto_OBJ_NeighborsValue2(float (*Op)(float, float),int i,int j,void ***readCells,float (*Neighbor_Value)(void*,void*),void* data);
void *Hauto_OBJ_FirstNeighbor(int i,int j,void ***readCells,float (*Neighbor_Value)(void*,void*),void* data);
#define NeighborsValue(Operation,Condition,Data) Hauto_OBJ_NeighborsValue(Operation,i,j,readcells,Condition,Data)
#define NeighborsValue2(Operation,Condition,Data) Hauto_OBJ_NeighborsValue2(Operation,i,j,readcells,Condition,Data)
#define FirstNeighbor(Condition,Data) Hauto_OBJ_FirstNeighbor(i,j,readcells,Condition,Data)
#define SetCell(i,j,Type,property,value) do{((Type*)automat->readCells[i][j])->property=value;((Type*)automat->writeCells[i][j])->property=value;}while(0)
#define GetCell(i,j,Type,property) ((Type*)automat->readCells[i][j])->property
//::::::::::::::::::::::::::::::::::::::::::://
#endif
/*................COMMENTS...................//
>>todo:

>>done:

>>others:

//..........................................*/

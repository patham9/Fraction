#ifndef Game_H
#define Game_H

#include "Hamlib/Hutil.h"
#include "Hamlib/Hauto.h"

#define WORLD 		  999
#define worldsize  	  256
#define ALLOW_SHADERS   1
#define MAX_HOUSES    200
#define MAX_PEOPLE	  100

#define BASE 999
#define TERRAFORM_DOWN 998
#define TERRAFORM_UP 997
uint STREET,ROCK,FOREST,HOUSE,WATER,GRASS,HOUSE_MEN,FOREST_MEN,GRASS_MEN,STREET_MEN,COMMAND,GPUTex,type;
Hauto_OBJ *automat;														//the world automat, read only
float **landscape;														//read only

void Game_Init();

#endif

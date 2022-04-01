#ifndef Game_H
#define Game_H

#include "Hamlib/Hutil.h"
#include "Hamlib/Hauto.h"

#define WORLD 		  1
#define worldsize  	  256
#define ALLOW_SHADERS   0
#define MAX_HOUSES    200
#define MAX_PEOPLE	  100
#define SINGLEPLAYER 	1
#define IP "91.203.212.130"
#define PORT 		10000
#define DIRECT_PLACEMENT 1

#define BASE 999 //needs to have biggest index
#define TERRAFORM_DOWN 998
#define TERRAFORM_UP 997
uint AGENT,BRIDGE,OR,XOR,AND,NEG,LOGIC_BEGIN,ROCK,OPENROCK,OFFSWITCH,SWITCH,OFFCURRENT,CURRENT,STREET,ROCK,FOREST,HOUSE,WATER,GRASS,HOUSE_MEN,FOREST_MEN,GRASS_MEN,STREET_MEN,COMMAND,GPUTex;
Hauto_OBJ *automat;														//the world automat, read only
float **landscape;														//read only

void Game_Init();

#endif

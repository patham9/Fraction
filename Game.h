#ifndef Game_H
#define Game_H

#include "Hamlib/Hutil.h"
#include "Hamlib/Hauto.h"

#define WORLD 999
#define worldsize 256

uint STREET,ROCK,FOREST,HOUSE,WATER,GRASS,GRASS_R,GRASS_L,GRASS_T,GRASS_D,GRASS_A,GRASS_RT,GRASS_LT,GRASS_LD,GRASS_RD,HOUSE_MEN,FOREST_MEN,GRASS_MEN,STREET_MEN,GPUTex,type;
Hauto_OBJ *automat;														//the world automat, read only
float **landscape;														//read only

void Game_Init();

#endif

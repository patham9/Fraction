#ifndef Game_H
#define Game_H

#include "Hamlib/Hutil.h"
#include "Hamlib/Hauto.h"

#define WORLD 999
#define worldsize 256

uint STREET,ROCK,FOREST,HOUSE,WATER,GRASS,HOUSE_MEN,FOREST_MEN,GRASS_MEN,STREET_MEN,GPUTex,type;
Hauto_OBJ *automat;														//the world automat, read only
float **landscape;														//read only

void Game_Init();

#endif

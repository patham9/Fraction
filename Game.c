#include "Game.h"
#include "Automat.h"
#include "Generate.h"
#include "Draw.h"
#include "gui.h"

void Game_Thread()
{
	while(1)
	{
		Hauto_OBJ_Exec(automat);
		Wait(0.001);
	}					
}
void Game_Init()
{
	////////////////////////// MAKE EVENT HANDLERS KNOWN TO HAMLIB //////////////////////////////////////////////
	hnav_SetRendFunc(draw);
	hrend_2Dmode(0.5,0.6,0.5);
	gui_Init();
	////////////////////////// LOAD TEXTURES ///////////////////////////////////////////////////////////////
	hfio_LoadTex("forest.tga",&FOREST);
	hfio_LoadTex("street.tga",&STREET);
	hfio_LoadTex("street_men.tga",&STREET_MEN);
	hfio_LoadTex("house.tga",&HOUSE);
	hfio_LoadTex("rock.tga",&ROCK);
	hfio_LoadTex("rock.tga",&GPUTex);
	hfio_LoadTex("house_men.tga",&HOUSE_MEN);
	hfio_LoadTex("grass_men.tga",&GRASS_MEN);
	hfio_LoadTex("forest_men.tga",&FOREST_MEN);
	WATER=102;GRASS=101;
	////////////////////////// OTHER INIT STUFF ///////////////////////////////////////////////////////////////
	type=GRASS;
	srand(WORLD);
	landscape=Generate_PerlinNoise(worldsize,worldsize,Generate_WhiteNoise(worldsize,worldsize),8,2);
	automat=Hauto_OBJ_NEW(10,worldsize,Automat_Simulate,Cell_NEW);
	Generate_World();
	Thread_NEW(Game_Thread,NULL);
}

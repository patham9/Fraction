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
	hfio_LoadTex("water.tga",&WATER);
	hfio_LoadTex("grass.tga",&GRASS); 
	hfio_LoadTex("water_grass_right.tga",&GRASS_L);
	hfio_LoadTex("water_grass_down.tga",&GRASS_T);
	hfio_LoadTex("water_grass_left.tga",&GRASS_R);
	hfio_LoadTex("water_grass_top.tga",&GRASS_D);
	hfio_LoadTex("water_grass_left_down.tga",&GRASS_RT);
	hfio_LoadTex("water_grass_left_top.tga",&GRASS_RD);
	hfio_LoadTex("water_grass_right_down.tga",&GRASS_LT);
	hfio_LoadTex("water_grass_right_top.tga",&GRASS_LD);
	hfio_LoadTex("water_grass_around.tga",&GRASS_A);
	hfio_LoadTex("grass.tga",&GPUTex);
	hfio_LoadTex("house_men.tga",&HOUSE_MEN);
	hfio_LoadTex("grass_men.tga",&GRASS_MEN);
	hfio_LoadTex("forest_men.tga",&FOREST_MEN);
	////////////////////////// OTHER INIT STUFF ///////////////////////////////////////////////////////////////
	type=GRASS;
	srand(WORLD);
	landscape=Generate_PerlinNoise(worldsize,worldsize,Generate_WhiteNoise(worldsize,worldsize),8,2);
	automat=Hauto_OBJ_NEW(10,worldsize,Automat_Simulate,Cell_NEW);
	Generate_World();
	Thread_NEW(Game_Thread,NULL);
}

#include "Hamlib/Hamlib.h"
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
	hfio_LoadTex("textures/command.tga",&COMMAND);
	hfio_LoadTex("textures/forest.tga",&FOREST);
	hfio_LoadTex("textures/street.tga",&STREET);
	hfio_LoadTex("textures/street_men.tga",&STREET_MEN);
	hfio_LoadTex("textures/house.tga",&HOUSE);
	hfio_LoadTex("textures/rock.tga",&ROCK);
	hfio_LoadTex("textures/rock.tga",&GPUTex);
	hfio_LoadTex("textures/house_men.tga",&HOUSE_MEN);
	hfio_LoadTex("textures/grass_men.tga",&GRASS_MEN);
	hfio_LoadTex("textures/forest_men.tga",&FOREST_MEN);
	WATER=102;GRASS=101;
	////////////////////////// OTHER INIT STUFF ///////////////////////////////////////////////////////////////
	type=BASE;
	srand(WORLD);
	landscape=Generate_PerlinNoise(worldsize,worldsize,Generate_WhiteNoise(worldsize,worldsize),8,0);
	automat=Hauto_OBJ_NEW(10,worldsize,Automat_Simulate,Cell_NEW);
	Generate_World();
	Thread_NEW(Game_Thread,NULL);
}
int main()
{
	Hamlib_CYCLIC(Game_Init,NULL,"1111111111111");
}

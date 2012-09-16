#include "Hamlib/Hamlib.h"
#include "Game.h"
#include "Automat.h"
#include "Generate.h"
#include "Draw.h"
#include "gui.h"
#include "Statistics.h"

void Game_Thread() 
{
	Statistics *s=statistics_new();
	int laststep=-1;
	while(1)
	{
		if(!SINGLEPLAYER && get_step()==laststep)
		{
			Wait(0.001);
			continue;
		}
		laststep=get_step();
		statistics_next(automat,s);
		Hauto_OBJ_Exec(automat,s);
		if(automat->t%10==0)
			printf("%f people\n",s->amount_of_people);
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
	automat=Hauto_OBJ_NEW(worldsize,Automat_Simulate,Cell_NEW);
	Generate_World();
	Thread_NEW(Game_Thread,NULL);
}
int main()
{
	Hamlib_CYCLIC(Game_Init,NULL,"1111111111111");
}

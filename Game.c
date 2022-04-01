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
	//while(1)
	{
		if(!SINGLEPLAYER && get_step()==laststep)
		{
			laststep=get_step();
			Wait(0.001);
			//continue;
			return;
		}
		set_finished(0);
		Wait(0.001);
		statistics_next(automat,s);
		Hauto_OBJ_Exec(automat,s);
		set_finished(1);
		if(automat->t%10==0)
			printf("%f people\n",s->amount_of_people);
		laststep=get_step();
		Wait(0.001);
	}					
}

int timecounter = 1;
void draw_and_progress()
{
	draw();
	//if(timecounter % 100 == 0)
	{
		Game_Thread();	
	}
}

void Game_Init()
{
	////////////////////////// MAKE EVENT HANDLERS KNOWN TO HAMLIB //////////////////////////////////////////////
	hnav_SetRendFunc(draw_and_progress);
	hrend_2Dmode(0.5,0.6,0.5);
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
    hfio_LoadTex("textures/and.tga",&AND);
    hfio_LoadTex("textures/neg.tga",&NEG);
    hfio_LoadTex("textures/xor.tga",&XOR);
    hfio_LoadTex("textures/or.tga",&OR);
    hfio_LoadTex("textures/openrock.tga",&OPENROCK);
    hfio_LoadTex("textures/offswitch.tga",&OFFSWITCH);
    hfio_LoadTex("textures/switch.tga",&SWITCH);
    hfio_LoadTex("textures/offcurrent.tga",&OFFCURRENT);
    hfio_LoadTex("textures/current.tga",&CURRENT);
    hfio_LoadTex("textures/rock.tga",&ROCK);
    hfio_LoadTex("textures/water.tga",&WATER);
    hfio_LoadTex("textures/agent.tga",&AGENT);
    hfio_LoadTex("textures/bridge.tga",&BRIDGE);
    hfio_LoadTex("textures/grass.tga",&GRASS);
	////////////////////////// OTHER INIT STUFF ///////////////////////////////////////////////////////////////
    gui_Init(); //since now buttons are involved GUI needs to be inited after texture loading
	srand(WORLD);
	landscape=Generate_PerlinNoise(worldsize,worldsize,Generate_WhiteNoise(worldsize,worldsize),8,0);
	automat=Hauto_OBJ_NEW(worldsize,Automat_Simulate,Cell_NEW);
	Generate_World();
	//Thread_NEW(Game_Thread,NULL);
}
int main()
{
	Hamlib_CYCLIC(Game_Init,NULL,"1111111111111");
}

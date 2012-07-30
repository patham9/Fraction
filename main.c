#include "Hamlib.h"														//hamlib include
#include "PerlinNoise.h"												//perlin noise generator include
#define RENDERMODE 0	//(0 and 2 recommended))						//0 slow GPU, 1 GPU (hm nearly only disadvantages at the moment but let it in for performance measurement reasons, 2 pixelshader, 3 3D pixelshader

static uint worldsize=256;												//so the cellular automat grid will be 500x500								
uint STREET=1,ROCK=2,FOREST=3,CITY=4,WATER=5,GRASS=-1,GRASS_R,GRASS_L,GRASS_T,GRASS_D,GRASS_A,GRASS_RT,GRASS_LT,GRASS_LD,GRASS_RD,GPUTex,MEN;//indexes for textures and cell states in one
uint type,shader;														//the cell type currently selected with the keys placed on mouse click
uint shader_state,shader_wateramount,shader_height,shader_i,shader_j,shader_t,shader_lastchange,shader_px,shader_py,shader_zoom;//shader uniform variables to the GPU
Hauto_OBJ *automat;														//the "object" simulating the cellular grid with its rules
float **landscape;														//initially storing generated height information for the landscape

Thread thread;
int running = 1;

#include "game.h"
#include "ai.h"
#include "automat.h"
#include "renderer.h"
#include "events.h"
#include "generator.h"
#include "init.h"

/* 
 * This part is to allow to end the program normally through kill -15
 * (gprof creates the gmon.out only on normal exit)
 */
#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
  if (signo == SIGTERM)
  {
	running = 0;
	glfwWaitThread(thread, 0);
	exit(0);
  }
}

int main()																//the main routine+
{
	// connect the signal handler for SIGTERM (15)
	if (signal(SIGTERM, sig_handler) == SIG_ERR);
	
	Hamlib_CYCLIC(init,NULL,"1111111111111");							//init hamlib
}

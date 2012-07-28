#include "Hamlib.h"														//hamlib include
#include "PerlinNoise.h"												//perlin noise generator include
#define RENDERMODE 2	//(0 and 2 recommended))						//0 slow GPU, 1 GPU (hm nearly only disadvantages at the moment but let it in for performance measurement reasons, 2 pixelshader, 3 3D pixelshader

static uint worldsize=512;												//so the cellular automat grid will be 500x500								
uint STREET=0,ROCK=1,FOREST=2,CITY=3,WATER=4,GRASS=-1,GRASS_R,GRASS_L,GRASS_T,GRASS_D,GRASS_A,GRASS_RT,GRASS_LT,GRASS_LD,GRASS_RD,GPUTex;//indexes for textures and cell states in one
uint type;																//the cell type currently selected with the keys placed on mouse click
uint shader_state,shader_wateramount,shader_height,shader_i,shader_j,shader_t,shader_lastchange,shader_difx,shader_dify,shader_zoom;//shader uniform variables to the GPU
Hauto_OBJ *automat;														//the "object" simulating the cellular grid with its rules
float **landscape;														//initially storing generated height information for the landscape

Thread thread;
int running = 1;

typedef struct
{
	int lastchange;														//when did the cell change the last time? procedural texture generator wants to know
	int state;															//cell type
	float wateramount;													//ground water
	void *rootwater;													//root pointer (from which water cell does the water cell come from)
	float height;														//the height of the cell, at first resulting from the landscape, but can change then
}Cell;

Cell *Cell_NEW(int i,int j) 		 									//constructor for a new cell called for every cell when the automat constructor is called
{
	Cell *ret=(Cell*)malloc(sizeof(Cell));								//allocating memory for the Cell pointer
	ret->state=GRASS;													//setting default state to GRASS
	ret->wateramount=0;													//no ground water by default
	ret->lastchange=0;													//
	ret->rootwater=NULL;												//no root pointer
	ret->height=landscape[i][j]*10.0;									//setting height to the landscape height at current position
	return ret;															//return our created object
}
float being_a(Cell *c,int state)									//a function returning true if the state of cell c equals state else false
{
	return c->state==state;
}
void Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	float water_amount(Cell *c,void *z)									
	{
		return c->wateramount;											//a function returning the ground water amount of a cell
	}
	float water_and_higher_than(Cell *c,float *height)					
	{
		return c->state==WATER && c->height>*height;					//a function which only returns true if the cell c is of type water, and higher than height
	}
	writeme->lastchange=readme->lastchange+1;
	writeme->state=readme->state;										//eliminating not defined behavior of not handled cases
	if(readme->state==WATER)																			
	{
		writeme->wateramount=1;											//water delivers maximum ground water to its surrounding
	}
	if(readme->state!=WATER)											//ground water also gets distributed to its neighborhood
	{
		writeme->wateramount=9*NeighborsValue(op_max,water_amount,NULL)/10; 
	} 
	if(readme->state==GRASS && NeighborsValue(op_plus,being_a,FOREST)==3 && NeighborsValue(op_plus,water_amount,NULL)/N>0.1) //(t%10==0 && .. to make it slower) also try 2 instead 3
	{
		writeme->state=FOREST;											//a cell which is grass and has 3 wood neighbours and enough ground water becomes forest
		writeme->lastchange=0;
	}
	if(readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,water_and_higher_than,&readme->height))
	{ 
		writeme->state=WATER; 											//(water flows downwards) a cell which isn't rock which has neighbors with water on a higher position gets water and remembers a root cell
		writeme->rootwater=FirstNeighbor(water_and_higher_than,&readme->height);
		writeme->lastchange=0;
	}
	if(readme->state==WATER && readme->rootwater!=NULL && ((Cell*)readme->rootwater)->state!=WATER)
	{
		writeme->state=GRASS;											//water without having a root water has lost its source
		writeme->rootwater=NULL; 
		writeme->lastchange=0;
	}
}

float *toGPU; //2D array of state, lastchange, height, wateramount
int btoGPU=0, Initial=1;
void draw()
{
	int i,j,k=0;
#if RENDERMODE!=1														
	for(j=automat->n-1;j>=0;j--)										//iterate through for(i=automat->n-1;i>0;i--)	
	{
		for(i=automat->n-1;i>=0;i--)									//the grid
		{	
#endif
#if RENDERMODE!=1
			Cell *c=((Cell*)automat->readCells[i][j]);	
#if RENDERMODE!=0
			toGPU[k]=1.0/(float)c->state; k++;
			toGPU[k]=1.0/(0.1+(float)c->lastchange); k++;
			toGPU[k]=1.0/(0.1+(float)c->height); k++;
			toGPU[k]=1.0/(0.1+(float)c->wateramount); k++;
#endif
#if RENDERMODE==0
			toGPU[k]=0.5+((Cell*)automat->readCells[i][j])->height/20.0; k++;									//use texture for rendering instead in mode 0
			toGPU[k]=0.6+((Cell*)automat->readCells[i][j])->height/20.0; k++;
			toGPU[k]=0.2+((Cell*)automat->readCells[i][j])->height/20.0+((Cell*)automat->readCells[i][j])->wateramount/5.0; k++;
			toGPU[k]=1.0; k++;
#endif
		}
	}
#endif
#if RENDERMODE!=1
#if RENDERMODE!=0
	glUniform1f(shader_t,(float)glfwGetTime());							//set shader time
	glUniform1f(shader_difx,hnav.difx);
	glUniform1f(shader_dify,hnav.dify);					
	glUniform1f(shader_zoom,hnav.zoom);
#endif
	glActiveTexture(GL_TEXTURE0);										//give data to GPU
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GPUTex);
	if(btoGPU || Initial)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, worldsize, worldsize, 0, GL_RGBA, GL_FLOAT, toGPU);
#if RENDERMODE!=0
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	//we need the values direct
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
#endif
#if RENDERMODE==0
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//we need the values direct
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
#endif
		btoGPU=0; Initial=0;
	}
#if RENDERMODE>=2
	glActiveTexture(GL_TEXTURE1);										//give textures to GPU
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GRASS);
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,CITY);
	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,FOREST);
	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ROCK);
	glActiveTexture(GL_TEXTURE5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,STREET);
	glActiveTexture(GL_TEXTURE6);
	glEnable(GL_TEXTURE_2D);
	hrend_DrawObj(worldsize/2-0.25,worldsize/2-0.25,0,worldsize/2,1,WATER);
#endif
#endif
#if RENDERMODE==0 || RENDERMODE==1
#if RENDERMODE==0
	hrend_SelectColor(0.45, 0.59, 0.31,1);
	hrend_DrawObj(worldsize/2-0.25,worldsize/2-0.25,0,worldsize/2,1,GPUTex);
#endif
	for(i=1;i<automat->n-1;i++)											//iterate through
	{
		for(j=1;j<automat->n-1;j++)										//the grid
		{	
			Cell ***readcells=(Cell***)automat->readCells;
			Cell *c=readcells[i][j];
			
#if RENDERMODE==0
			if(c->state!=GRASS)
#endif			
			{															//draw cell only if camera sees, wouldn't make sense else
				if(abs(hnav_MouseToWorldCoordX(hrend.width/2)-i)<hnav_MouseToWorldCoordX(hrend.width/2)-hnav_MouseToWorldCoordX(0) 	
				&& abs(hnav_MouseToWorldCoordY(hrend.height/2)-j)<hnav_MouseToWorldCoordY(0)-hnav_MouseToWorldCoordY(hrend.height/2))
				{															//select color and draw
					int grass_around=NeighborsValue(op_plus,being_a,GRASS);
					hrend_SelectColor(0.5+c->height/20.0,0.6+c->height/20.0,0.2+c->height/20.0+c->wateramount/5.0,1);
					if(c->state==WATER && grass_around>=3)
					{
						if(grass_around>=8)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_A);
						}
						else
						if(readcells[i+1][j]->state==GRASS && readcells[i][j+1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_RT);
						}
						else
						if(readcells[i-1][j]->state==GRASS && readcells[i][j-1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_LD);
						}
						else
						if(readcells[i+1][j]->state==GRASS && readcells[i][j-1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_RD);
						}
						else
						if(readcells[i-1][j]->state==GRASS && readcells[i][j+1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_LT);
						}
						else
						if(readcells[i+1][j]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_R);
						}
						else
						if(readcells[i-1][j]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_L);
						}
						else
						if(readcells[i][j+1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_T);
						}
						else
						if(readcells[i][j-1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_D);
						}
						else
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS);
						}
					}
					else
						hrend_DrawObj(i,j,0,0.5,1,c->state);
				}
			}
		}
	}
#endif
	Wait(0.001);
}

void mouse_down(EventArgs *e)
{																		//get world coordinates of cursor and check if it is in the automats range
	int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5); 
	if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))
	{
		if(e->mk==1)
		{
			SetCell(i,j,Cell,state,type); SetCell(i,j,Cell,rootwater,NULL);	//set the cell to the selected type on left mouse
		}																
	}
}

void key_up(EventArgs*e) 
{
	if (e->mk=='X')
	{
		// running will end the main loop of the thread
		// then we wait and exit normally
		running = 0;
		glfwWaitThread(thread, 0);
		exit(0);
	}
	if(e->mk=='W')														
	{
		type=WATER;														//select the types for the placement of objects with keys
	}																	
	if(e->mk=='R')
	{
		type=ROCK;														//select the types for the placement of objects with keys
	}
	if(e->mk=='C')
	{
		type=CITY;														//select the types for the placement of objects with keys
	}
	if(e->mk=='F')
	{
		type=FOREST;													//select the types for the placement of objects with keys
	}
	if(e->mk=='G')														
	{
		type=GRASS;														//select the types for the placement of objects with keys
	}
	if(e->mk=='S')														
	{
		type=STREET;													//select the types for the placement of objects with keys
	}
}

void Automat_Thread()
{
	while(running)
	{
		Hauto_OBJ_Exec(automat);										//update automat state and 
#if RENDERMODE!=0														
		btoGPU=1;
#endif
		Wait(0.001);													//wait a bit to not consume max CPU core time
	}					
}

void GenerateNature()
{
	int i,j;
	for(i=2;i<worldsize-2;i++)											//iterate through
	{
		for(j=2;j<worldsize-2;j++)										//the grid
		{	
			if(drnd()>0.999940)											
			{
				SetCell(i,j,Cell,state,WATER);							//maybe place some water
			}
			if(drnd()>0.9990)
			{
				SetCell(i,j,Cell,state,FOREST);							//maybe some forest
				SetCell(i+1,j,Cell,state,FOREST);
				SetCell(i,j+1,Cell,state,FOREST);
				SetCell(i-1,j,Cell,state,FOREST);
			}
		}
	}
}

void init()  
{			
	uint i,j,shader;
	hfio_LoadTex("forest.tga",&FOREST);									//load forest texture
	hfio_LoadTex("house.tga",&CITY);									//load city texture
	hfio_LoadTex("street.tga",&STREET);									//load street texture
	hfio_LoadTex("rock.tga",&ROCK);										//load rock texture
	hfio_LoadTex("water.tga",&WATER);									//load water texture
	hfio_LoadTex("grass.tga",&GRASS);									//load grass texture
	hfio_LoadTex("water_grass_right.tga",&GRASS_L);						//load grass texture
	hfio_LoadTex("water_grass_down.tga",&GRASS_T);						//load grass texture
	hfio_LoadTex("water_grass_left.tga",&GRASS_R);						//load grass texture
	hfio_LoadTex("water_grass_top.tga",&GRASS_D);						//load grass texture
	hfio_LoadTex("water_grass_left_down.tga",&GRASS_RT);				//load grass texture
	hfio_LoadTex("water_grass_left_top.tga",&GRASS_RD);					//load grass texture
	hfio_LoadTex("water_grass_right_down.tga",&GRASS_LT);				//load grass texture
	hfio_LoadTex("water_grass_right_top.tga",&GRASS_LD);				//load grass texture
	hfio_LoadTex("water_grass_around.tga",&GRASS_A);					//load grass texture
#if RENDERMODE==2 || RENDERMODE==3
#if RENDERMODE==2
	shader=hshade_CreateShaderPair("vertexshader","pixelshader");  		//load pixel (and vertex) shader	
#endif	
#if RENDERMODE==3
	shader=hshade_CreateShaderPair("vertexshader3d","pixelshader3d");  	//load pixel (and vertex) shader	
#endif				
	glUseProgram(shader);												//use pixel (and vertex) shader
	hfio_LoadTex("grass.tga",&GPUTex);									//ehm
	glUniform1i(glGetUniformLocation(shader, "data"), 0);
	glUniform1i(glGetUniformLocation(shader, "grass_texture"), 1);
	glUniform1i(glGetUniformLocation(shader, "city_texture"), 2);
	glUniform1i(glGetUniformLocation(shader, "forest_texture"), 3);
	glUniform1i(glGetUniformLocation(shader, "rock_texture"), 4);
	glUniform1i(glGetUniformLocation(shader, "street_texture"), 5);
	glUniform1i(glGetUniformLocation(shader, "water_texture"), 6);
	shader_t=glGetUniformLocation(shader, "t");							//time
	shader_difx=glGetUniformLocation(shader, "difx");
	shader_dify=glGetUniformLocation(shader, "dify");
	shader_zoom=glGetUniformLocation(shader, "zoom");
	glUniform1f(glGetUniformLocation(shader, "ROCK"),1.0/(float)ROCK);				//make GPU know of our state constants
	glUniform1f(glGetUniformLocation(shader, "FOREST"),1.0/(float)FOREST);			
	glUniform1f(glGetUniformLocation(shader, "CITY"),1.0/(float)CITY);
	glUniform1f(glGetUniformLocation(shader, "WATER"),1.0/(float)WATER);
	glUniform1f(glGetUniformLocation(shader, "GRASS"),1.0/(float)GRASS);
	glUniform1f(glGetUniformLocation(shader, "STREET"),1.0/(float)STREET);
	glUniform1i(glGetUniformLocation(shader, "worldsize"),worldsize);	
#endif
	toGPU=(float*)malloc(worldsize*worldsize*4*sizeof(float));	
	hnav_SetRendFunc(draw);												//set hamlib render routine
	hrend_2Dmode(0.5,0.6,0.5);											//set hamlib render mode to 2D
	hinput_AddMouseDown(mouse_down);									//add mouse down event
	hinput_AddMouseDragged(mouse_down);									//add mouse dragged event
	hinput_AddKeyUp(key_up);											//add key up event
	type=GRASS;															//set default object to place on click to grass
	srand(1);															//random generator seed. use current time and world will be different on every execution
	landscape=GeneratePerlinNoise(worldsize,worldsize,WhiteNoise(worldsize,worldsize),8,2);	//generate landscape
	automat=Hauto_OBJ_NEW(10,worldsize,Simulate,Cell_NEW);				//create cellular automat object
	GenerateNature();													//generate the nature on it
	thread = Thread_NEW(Automat_Thread,NULL);									//create a thread for the automat where it can execute on its own core then
}

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

int main()																//the main routine
{
	// connect the signal handler for SIGTERM (15)
	if (signal(SIGTERM, sig_handler) == SIG_ERR);
	
	Hamlib_CYCLIC(init,NULL,"1111111111111");							//init hamlib
}

#include "Hamlib.h"														//hamlib include
#include "PerlinNoise.h"												//perlin noise generator include
#define RENDERMODE 1			//0 CPU, 1 GPU, 2 GPU 3D (pixelshader-based offset mapping etc.)

static uint worldsize=512;												//so the cellular automat grid will be 500x500								
uint ROCK=1,FOREST=2,CITY=3,WATER=4,GRASS=-1,GPUTex;					//indexes for textures and cell states in one 
uint type;																//the cell type currently selected with the keys placed on mouse click
uint shader_state,shader_wateramount,shader_height,shader_i,shader_j,shader_t,shader_lastchange,shader_difx,shader_dify,shader_zoom;//shader uniform variables to the GPU
Hauto_OBJ *automat;														//the "object" simulating the cellular grid with its rules
float **landscape;														//initially storing generated height information for the landscape

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

void Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	float being_a(Cell *c,int state)									//a function returning true if the state of cell c equals state else false
	{
		return c->state==state;
	}
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
		writeme->rootwater=FirstNeigbor(water_and_higher_than,&readme->height);
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
int btoGPU=0;
void draw()
{
	int i,j,k=0;
#if RENDERMODE==1
	for(i=0;i<automat->n;i++)											//iterate through
	{
		for(j=0;j<automat->n;j++)										//the grid
		{	
			Cell *c=((Cell*)automat->readCells[i][j]);	
			toGPU[k]=1.0/(float)c->state; k++;
			toGPU[k]=1.0/(0.1+(float)c->lastchange); k++;
			toGPU[k]=1.0/(0.1+(float)c->height); k++;
			toGPU[k]=1.0/(0.1+(float)c->wateramount); k++;
		}
	}
	glUniform1f(shader_t,(float)glfwGetTime());							//set shader time
	glUniform1f(shader_difx,hnav.difx);
	glUniform1f(shader_dify,hnav.dify);					
	glUniform1f(shader_zoom,hnav.zoom);
	glActiveTexture(GL_TEXTURE0);										//give data to GPU
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GPUTex);
	if(btoGPU)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, worldsize, worldsize, 0, GL_RGBA, GL_FLOAT, toGPU);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	//we need the values direct
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		btoGPU=0;
	}
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
	hrend_DrawObj(worldsize/2-0.25,worldsize/2-0.25,0,worldsize/2,1,WATER);
	Wait(0.001);														//wait a bit to not consume max. CPU
#endif
#if RENDERMODE==0
	for(i=1;i<automat->n-1;i++)											//iterate through
	{
		for(j=1;j<automat->n-1;j++)										//the grid
		{																//draw cell only if camera sees, wouldn't make sense else
			if(abs(hnav_MouseToWorldCoordX(hrend.width/2)-i)<hnav_MouseToWorldCoordX(hrend.width/2)-hnav_MouseToWorldCoordX(0) 	
		    && abs(hnav_MouseToWorldCoordY(hrend.height/2)-j)<hnav_MouseToWorldCoordY(0)-hnav_MouseToWorldCoordY(hrend.height/2))
			{															//select color and draw
				hrend_SelectColor(0.5+((Cell*)automat->readCells[i][j])->height/20.0,0.6+((Cell*)automat->readCells[i][j])->height/20.0,0.2+((Cell*)automat->readCells[i][j])->height/20.0+((Cell*)automat->readCells[i][j])->wateramount/5.0,1);
				hrend_DrawObj(i,j,0,0.5,1,((Cell*)automat->readCells[i][j])->state);
			}
		}
	}
	Wait(0.001);
#endif
}

void mouse_down(EventArgs *e)
{																		//get world coordinates of cursor and check if it is in the automats range
	int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5); 
	if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))
	{
		if(e->mk==1)
		{
			SetCell(i,j,Cell,state,type);								//set the cell to the selected type on left mouse
		}																
	}
}

void key_up(EventArgs*e) 
{
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
}

void Automat_Thread()
{
	while(1)
	{
		Hauto_OBJ_Exec(automat);										//update automat state and 
		btoGPU=1;
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
			if(frnd()>0.9998)											
			{
				SetCell(i,j,Cell,state,WATER);							//maybe place some water
			}
			if(frnd()>0.9990)
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
	hfio_LoadTex("city.tga",&CITY);										//load city texture
	hfio_LoadTex("rock.tga",&ROCK);										//load rock texture
	hfio_LoadTex("water.tga",&WATER);									//load water texture
	hfio_LoadTex("grass.tga",&GRASS);									//load grass texture
#if RENDERMODE==1 || RENDERMODE==2
#if RENDERMODE==1
	shader=hshade_CreateShaderPair("vertexshader","pixelshader");  		//load pixel (and vertex) shader	
#endif	
#if RENDERMODE==2
	shader=hshade_CreateShaderPair("vertexshader3d","pixelshader3d");  	//load pixel (and vertex) shader	
#endif				
	glUseProgram(shader);												//use pixel (and vertex) shader
	hfio_LoadTex("grass.tga",&GPUTex);									//ehm
	glUniform1i(glGetUniformLocation(shader, "data"), 0);
	glUniform1i(glGetUniformLocation(shader, "grass_texture"), 1);
	glUniform1i(glGetUniformLocation(shader, "city_texture"), 2);
	glUniform1i(glGetUniformLocation(shader, "forest_texture"), 3);
	glUniform1i(glGetUniformLocation(shader, "rock_texture"), 4);
	glUniform1i(glGetUniformLocation(shader, "water_texture"), 5);
	shader_t=glGetUniformLocation(shader, "t");							//time
	shader_difx=glGetUniformLocation(shader, "difx");
	shader_dify=glGetUniformLocation(shader, "dify");
	shader_zoom=glGetUniformLocation(shader, "zoom");
	glUniform1f(glGetUniformLocation(shader, "ROCK"),1.0/(float)ROCK);				//make GPU know of our state constants
	glUniform1f(glGetUniformLocation(shader, "FOREST"),1.0/(float)FOREST);			
	glUniform1f(glGetUniformLocation(shader, "CITY"),1.0/(float)CITY);
	glUniform1f(glGetUniformLocation(shader, "WATER"),1.0/(float)WATER);
	glUniform1f(glGetUniformLocation(shader, "GRASS"),1.0/(float)GRASS);
	glUniform1i(glGetUniformLocation(shader, "worldsize"),worldsize);
	toGPU=(float*)malloc(worldsize*worldsize*4*sizeof(float));		
#endif
	hnav_SetRendFunc(draw);												//set hamlib render routine
	hrend_2Dmode(0.5,0.6,0.5);											//set hamlib render mode to 2D
	hinput_AddMouseDown(mouse_down);									//add mouse down event
	hinput_AddMouseDragged(mouse_down);									//add mouse dragged event
	hinput_AddKeyUp(key_up);											//add key up event
	type=GRASS;															//set default object to place on click to grass
	srand(999);															//random generator seed. use current time and world will be different on every execution
	landscape=GeneratePerlinNoise(worldsize,worldsize,WhiteNoise(worldsize,worldsize),8);	//generate landscape
	automat=Hauto_OBJ_NEW(10,worldsize,Simulate,Cell_NEW);				//create cellular automat object
	GenerateNature();													//generate the nature on it
	Thread_NEW(Automat_Thread,NULL);									//create a thread for the automat where it can execute on its own core then
}

int main()																//the main routine
{
	Hamlib_CYCLIC(init,NULL,"1111111111111");							//init hamlib
}

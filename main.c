#include "Hamlib.h"														//hamlib include
#include "PerlinNoise.h"												//perlin noise generator include

uint worldsize=500;														//so the cellular automat grid will be 500x500								
uint ROCK=0,FOREST=1,CITY=2,WATER=3,GRASS=-1;							//indexes for textures and cell states in one 
uint type;																//the cell type currently selected with the keys placed on mouse click
uint shader_state,shader_wateramount,shader_height,shader_i,shader_j,shader_t;//the shader uniform variables which get to the GPU
Hauto_OBJ *automat;														//the "object" simulating the cellular grid with its rules
float **landscape;														//initially storing generated height information for the landscape

typedef struct
{
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
	ret->rootwater=NULL;												//no root pointer
	ret->height=landscape[i][j];										//setting height to the landscape height at current position
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
	} 
	
	if(readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,water_and_higher_than,&readme->height))
	{ 
		writeme->state=WATER; 											//(water flows downwards) a cell which isn't rock which has neighbors with water on a higher position gets water and remembers a root cell
		writeme->rootwater=FirstNeigbor(water_and_higher_than,&readme->height);
	} 
	
	if(readme->state==WATER && readme->rootwater!=NULL && ((Cell*)readme->rootwater)->state!=WATER)
	{
		writeme->state=GRASS;											//water without having a root water has lost its source
		writeme->rootwater=NULL; 
	}
}

void draw()
{
	int i,j;
	for(i=1;i<automat->n-1;i++)											//iterate through
	{
		for(j=1;j<automat->n-1;j++)										//the grid
		{																//draw cell only if camera sees, wouldn't make sense else
			if(abs(hnav_MouseToWorldCoordX(hrend.width/2)-i)<hnav_MouseToWorldCoordX(hrend.width/2)-hnav_MouseToWorldCoordX(0) 	
		    && abs(hnav_MouseToWorldCoordY(hrend.height/2)-j)<hnav_MouseToWorldCoordY(0)-hnav_MouseToWorldCoordY(hrend.height/2))
			{															//select color and draw
				Cell *c=((Cell*)automat->readCells[i][j]);							
				glUniform1i(shader_state,c->state);						//give variables to GPU
				glUniform1i(shader_wateramount,c->wateramount);
				glUniform1f(shader_height,c->height);
				glUniform1i(shader_i,i);
				glUniform1i(shader_j,j);
				glUniform1f(shader_t,(float)glfwGetTime());
				hrend_SelectColor(0.5+c->height/1.5,0.4+c->height/1.5,0.2+c->height/1.5+c->wateramount/5.0,1);
				hrend_DrawObj(i,j,0,0.5,1,c->state);
			}
		}
	}
	Wait(0.001);														//wait a bit to not consume max. CPU
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
	uint shader=hshade_CreateShaderPair("vertexshader","pixelshader");  //load pixel (and vertex) shader	
	shader_state,shader_wateramount,shader_height,shader_i,shader_j;	//get GPU "variable pointers"					
	shader_state=glGetUniformLocation(shader, "state");
	shader_height=glGetUniformLocation(shader, "height");
	shader_i=glGetUniformLocation(shader, "i");
	shader_j=glGetUniformLocation(shader, "j");
	shader_t=glGetUniformLocation(shader, "t");							//time
	shader_wateramount=glGetUniformLocation(shader, "wateramount");	
	glUseProgram(shader);												//use pixel (and vertex) shader
	hnav_SetRendFunc(draw);												//set hamlib render routine
	hrend_2Dmode(0.5,0.6,0.5);											//set hamlib render mode to 2D
	hinput_AddMouseDown(mouse_down);									//add mouse down event
	hinput_AddMouseDragged(mouse_down);									//add mouse dragged event
	hinput_AddKeyUp(key_up);											//add key up event
	hfio_LoadTex("forest.tga",&FOREST);									//load forest texture
	hfio_LoadTex("city.tga",&CITY);										//load city texture
	hfio_LoadTex("rock.tga",&ROCK);										//load rock texture
	hfio_LoadTex("water.tga",&WATER);									//load water texture
	hfio_LoadTex("grass.tga",&GRASS);									//load grass texture
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

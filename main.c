#include "Hamlib.h"
#include "PerlinNoise.h"
uint worldsize=500,ROCK=0,FOREST=1,CITY=2,WATER=3,GRASS=-1,type;		//constants for world size, states (and textures at the same time)
Hauto_OBJ *automat;														//the world automat
float **landscape;
typedef struct{ int state; float wateramount; void *rootwater; float height; }Cell;
Cell *Cell_NEW(int i,int j) 		 									//constructor for a new cell called for every cell when the automat constructor is called
{
	Cell *ret=(Cell*)malloc(sizeof(Cell));
	ret->state=GRASS; ret->wateramount=0; ret->rootwater=NULL; ret->height=landscape[i][j];
	return ret;
}
void Simulate(int t,int i,int j,Cell *writeme,Cell* readme,Cell* left,Cell* right,Cell* up,Cell* down,Cell* left_up,Cell* left_down,Cell* right_up,Cell* right_down,Cell ***readcells)
{
	float being_a(Cell *c,int state){return c->state==state;}
	float water_amount(Cell *c,void *z){return c->wateramount;}
	float water_and_higher_than(Cell *c,float *height){return c->state==WATER && c->height>*height;}
	writeme->state=readme->state;
	//water delivers maximum ground water:
	if(readme->state==WATER){ writeme->wateramount=1; } 					
	//ground water floods neighborhood:
	if(readme->state!=WATER){ writeme->wateramount=9*NeighborsValue(op_max,water_amount,NULL)/10; } 
	//a cell which is grass and has 3 wood neighbours and ground water becomes forest: (t%10==0 && to make it slower)
	if(readme->state==GRASS && NeighborsValue(op_plus,being_a,FOREST)==3 && NeighborsValue(op_plus,water_amount,NULL)/N>0.1){ writeme->state=FOREST; } //also try 2 instead 3
	//a cell which isn't rock which has neighbors with water on a higher position gets water and remembers a root cell (water flows downwards)
	if(readme->state!=WATER && readme->state!=ROCK && NeighborsValue(op_or,water_and_higher_than,&readme->height)){ writeme->state=WATER; writeme->rootwater=FirstNeigbor(water_and_higher_than,&readme->height);} 
	//water without having a root water has lost its source
	if(readme->state==WATER && readme->rootwater!=NULL && ((Cell*)readme->rootwater)->state!=WATER){ writeme->state=GRASS; writeme->rootwater=NULL; }
}
void draw()
{
	int i,j;
	for(i=1;i<automat->n-1;i++)
	{
		for(j=1;j<automat->n-1;j++)
		{																//draw cell only if camera sees it and if state!=GRASS
			if(abs(hnav_MouseToWorldCoordX(hrend.width/2)-i)<hnav_MouseToWorldCoordX(hrend.width/2)-hnav_MouseToWorldCoordX(0) 	
		    && abs(hnav_MouseToWorldCoordY(hrend.height/2)-j)<hnav_MouseToWorldCoordY(0)-hnav_MouseToWorldCoordY(hrend.height/2))
			{
				hrend_SelectColor(0.5+((Cell*)automat->readCells[i][j])->height/1.5,0.4+((Cell*)automat->readCells[i][j])->height/1.5,0.2+((Cell*)automat->readCells[i][j])->height/1.5+((Cell*)automat->readCells[i][j])->wateramount/5.0,1);
				hrend_DrawObj(i,j,0,0.5,1,((Cell*)automat->readCells[i][j])->state);
			}
		}
	}
	Wait(0.001);
}
void mouse_down(EventArgs *e)
{																		//get world coordinates of cursor and check if it is in automat range
	int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5); 
	if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))
	{
		if(e->mk==1){ SetCell(i,j,Cell,state,type); }					//set the cell to the selected type on left mouse
	}
}
void key_up(EventArgs*e) 
{
	if(e->mk=='W'){type=WATER;}											//select the types for the placement of objects with keys
	if(e->mk=='R'){type=ROCK;}
	if(e->mk=='C'){type=CITY;}
	if(e->mk=='F'){type=FOREST;}
	if(e->mk=='G'){type=GRASS;}
}
void Automat_Thread()
{
	while(1){ Hauto_OBJ_Exec(automat); Wait(0.001); }					//update automat state and wait a bit
}
void GenerateNature()
{
	int i,j;
	for(i=2;i<worldsize-2;i++)
	{
		for(j=2;j<worldsize-2;j++)
		{
			if(frnd()>0.9998){SetCell(i,j,Cell,state,WATER);}
			if(frnd()>0.9990){SetCell(i,j,Cell,state,FOREST);SetCell(i+1,j,Cell,state,FOREST);SetCell(i,j+1,Cell,state,FOREST);SetCell(i-1,j,Cell,state,FOREST);}
		}
	}
}
void init()  
{																		//make the functions known to Hamlib, load textures, and other init stuff:
	hnav_SetRendFunc(draw); hrend_2Dmode(0.5,0.6,0.5); hinput_AddMouseDown(mouse_down);hinput_AddMouseDragged(mouse_down); hinput_AddKeyUp(key_up);
	hfio_LoadTex("forest.tga",&FOREST); hfio_LoadTex("city.tga",&CITY); hfio_LoadTex("rock.tga",&ROCK); hfio_LoadTex("water.tga",&WATER); hfio_LoadTex("grass.tga",&GRASS); type=GRASS;
	landscape=WhiteNoise(worldsize,worldsize); srand(999);
	landscape=GeneratePerlinNoise(worldsize,worldsize,WhiteNoise(worldsize,worldsize),8); //10
	automat=Hauto_OBJ_NEW(10,worldsize,Simulate,Cell_NEW);
	GenerateNature();
	Thread_NEW(Automat_Thread,NULL);
}
int main()
{
	Hamlib_CYCLIC(init,NULL,"1111111111111");							//init hamlib
}

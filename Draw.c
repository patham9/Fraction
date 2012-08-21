#include "Draw.h"
#include "Game.h"
#include "Automat.h"
#include "Cell.h"

int debugdraw=debugdraw_enabled_default; //global variable but just for debugging
void Draw_Set_DebugDraw(int s)
{
	debugdraw=s;
}
int Draw_Get_DebugDraw()
{
	return debugdraw;
}
int HeightmapChanged=1;										//something we need to discuss! I would prefer letting it out but it gives a lot of performance
void Draw_Set_HeightmapChanged()
{
	HeightmapChanged=1;
}
void draw()
{
	int i,j,k=0;
	hrend_SelectColor(0.45, 0.59, 0.31,1);
	hrend_DrawObj(worldsize/2-0.25,worldsize/2-0.25,0,worldsize/2,1,GPUTex);
	float *toGPU=(float*)malloc(worldsize*worldsize*4*sizeof(float));	//maybe letter not here but i don't want it global!
	for(j=automat->n-1;j>=0;j--)										//iterate through for(i=automat->n-1;i>0;i--)	
	{
		for(i=automat->n-1;i>=0;i--)									//the grid
		{	
			Cell *c=((Cell*)automat->readCells[i][j]);
			if(c->state!=WATER)
			{
				toGPU[k]=0.5+((Cell*)automat->readCells[i][j])->height/15.0; k++;	//use texture for rendering instead in mode 0
				toGPU[k]=0.6+((Cell*)automat->readCells[i][j])->height/15.0; k++;
				toGPU[k]=0.2+((Cell*)automat->readCells[i][j])->height/15.0+((Cell*)automat->readCells[i][j])->wateramount/5.0; k++;
			}
			else
			{
				toGPU[k]=0.5; k++;	//use texture for rendering instead in mode 0
				toGPU[k]=0.7; k++;
				toGPU[k]=1; k++;
			}
			toGPU[k]=1.0; k++;
		}
	}
	glActiveTexture(GL_TEXTURE0);										//give data to GPU
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GPUTex);
	if(HeightmapChanged)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, worldsize, worldsize, 0, GL_RGBA, GL_FLOAT, toGPU);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//we need the values direct
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		HeightmapChanged=0;
	}
	for(i=1;i<automat->n-1;i++)
	{
		for(j=1;j<automat->n-1;j++)
		{		
			Cell ***readcells=(Cell***)automat->readCells;
			Cell *c=readcells[i][j];
			if(debugdraw || (c->state!=GRASS && c->state!=WATER) || c->person)											//draw cell only if camera sees it and if state!=GRASS
			{														
				if(abs(hnav_MouseToWorldCoordX(hrend.width/2)-i)<hnav_MouseToWorldCoordX(hrend.width/2)-hnav_MouseToWorldCoordX(0) 	
				&& abs(hnav_MouseToWorldCoordY(hrend.height/2)-j)<hnav_MouseToWorldCoordY(0)-hnav_MouseToWorldCoordY(hrend.height/2))
				{	
					float addr=0.0,addg=0.0,addb=0.0;
					if(debugdraw==1)
					{
						addr=debugdraw_property_r;
						addg=debugdraw_property_g; 
						addb=debugdraw_property_b;
					}
					int grass_around=NeighborsValue(op_plus,being_a,GRASS);
					hrend_SelectColor(addr+0.5+c->height/20.0,addg+0.6+c->height/20.0,addb+0.2+c->height/20.0+c->wateramount/5.0,1);
					
					if(c->person)
					{
						if(c->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_MEN);
						}
						if(c->state==HOUSE)
						{
							hrend_DrawObj(i,j,0,0.5,1,HOUSE_MEN);
						}
						if(c->state==FOREST)
						{
							hrend_DrawObj(i,j,0,0.5,1,FOREST_MEN);
						}
						if(c->state==STREET)
						{
							hrend_DrawObj(i,j,0,0.5,1,STREET_MEN);
						}
					}
					else
					{
						hrend_DrawObj(i,j,0,0.5,1,c->state);
					}
				}
			}
		}
	}
	Wait(0.001);
	free(toGPU);
}

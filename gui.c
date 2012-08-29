#include "gui.h"
#include "Cell.h"
#include "Game.h"
#include "Draw.h"

enum{none,leftup,rightdown,copy,paste} mouse_operation;
int lux=0,luy=0,rdx=0,rdy=0,sx=0,sy=0,*data;
void mouse_down(EventArgs *e)
{		
	if(e->mk==1)
	{																
		int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5),x,y; 
		if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))			//get world coordinates of cursor and check if it is in automat range
		{
			if(mouse_operation==none)
			{
				if(type==BASE)
				{
					SetCell(i,j,Cell,state,HOUSE);						//set the cell to the selected type on left mouse
					SetCell(i,j,Cell,person,worker);					//also place someone
				}
				else
				{
					SetCell(i,j,Cell,command,type);
					SetCell(i,j,Cell,has_command,1);
				}
			}
			if(mouse_operation==leftup)
			{
				lux=i; luy=j;
			}
			if(mouse_operation==rightdown)
			{
				rdx=i; rdy=j;
			}
			if(mouse_operation==paste)
			{
				if(data!=NULL && i>0 && j>0 && i+sx<automat->n-1 && j+sy<automat->n-1 && sx>0 && sy>0)
				{
					for(x=0;x<sx;x++)
					{
						for(y=0;y<sy;y++)
						{
							if(data[x*sy+y]!=GetCell(i+x,j+y,Cell,state))
							{
								SetCell(i+x,j+y,Cell,has_command,1);
								SetCell(i+x,j+y,Cell,command,data[x*sy+y]);
							}
						}
					}	
				}
			}
		}			
	}
}
void key_up(EventArgs *e) 
{
	if(e->mk>'A' && e->mk<'Z')
		mouse_operation=none;
	if(e->mk=='W'){ type=WATER; }
	if(e->mk=='R'){ type=ROCK; }
	if(e->mk=='H'){ type=HOUSE; }
	if(e->mk=='F'){ type=FOREST; }
	if(e->mk=='G'){ type=GRASS; }
	if(e->mk=='S'){ type=STREET; }
	if(e->mk=='B'){ type=BASE; }
	if(e->mk=='D'){ Draw_Set_DebugDraw(!Draw_Get_DebugDraw()); }
	if(e->mk=='P'){ mouse_operation=paste; }
}
void Button(int i)
{
	int x,y;
	if(i==none || i==leftup || i==rightdown || i==copy || i==paste)
	{
		mouse_operation=i;
		if(mouse_operation==copy)
		{	
			sx=rdx-lux; sy=luy-rdy;
			if(data!=NULL) free(data);
			data=(int*)calloc(1,sx*sy*sizeof(int));
			for(x=0;x<sx;x++)
			{
				for(y=0;y<sy;y++)
				{
					data[x*sy+y]=GetCell(x+lux,y+rdy,Cell,state);
				}
			}
		}
	}
	else
	{
		key_up(&(EventArgs){0,0,i});
	}
}
void gui_Init()
{
	hinput_AddMouseDown(mouse_down);
	hinput_AddMouseDragged(mouse_down);
	hinput_AddKeyUp(key_up);
	hgui_AddSimpleElem(0.0,0,0.09,0.04,"EXIT",exit,0);
	hgui_AddSimpleElem(0.1,0,0.09,0.04,"WATER",Button,'W');
	hgui_AddSimpleElem(0.2,0,0.09,0.04,"FOREST",Button,'F');
	hgui_AddSimpleElem(0.3,0,0.09,0.04,"STREET",Button,'S');
	hgui_AddSimpleElem(0.4,0,0.09,0.04,"GRASS",Button,'G');
	hgui_AddSimpleElem(0.5,0,0.09,0.04,"ROCK",Button,'R');
	hgui_AddSimpleElem(0.6,0,0.09,0.04,"HOUSE",Button,'H');
	hgui_AddSimpleElem(0.7,0,0.09,0.04,"DEBUG",Button,'D');
	hgui_AddSimpleElem(0.8,0,0.09,0.04,"BASE",Button,'B');
	hgui_AddSimpleElem(0.0,0.96,0.09,0.04,"LEFTUP",Button,leftup);
	hgui_AddSimpleElem(0.1,0.96,0.09,0.04,"RIDOWN",Button,rightdown);
	hgui_AddSimpleElem(0.2,0.96,0.09,0.04,"COPY",Button,copy);
	hgui_AddSimpleElem(0.3,0.96,0.09,0.04,"PASTE",Button,paste);
}

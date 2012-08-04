#include "gui.h"
#include "Cell.h"
#include "Game.h"
#include "Draw.h"

void mouse_down(EventArgs *e)
{																		
	int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5); 
	if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))				//get world coordinates of cursor and check if it is in automat range
	{
		if(e->mk==1)
		{
			SetCell(i,j,Cell,state,type);								//set the cell to the selected type on left mouse
		}					
	}
}
void key_up(EventArgs *e) 
{
	if(e->mk=='W'){ type=WATER; }
	if(e->mk=='R'){ type=ROCK; }
	if(e->mk=='H'){ type=HOUSE; }
	if(e->mk=='F'){ type=FOREST; }
	if(e->mk=='G'){ type=GRASS; }
	if(e->mk=='S'){ type=STREET; }
	if(e->mk=='D'){ Draw_Set_DebugDraw(!Draw_Get_DebugDraw()); }
}
void Button(int i)
{
	key_up(&(EventArgs){0,0,i});
}
void gui_Init()
{
	hinput_AddMouseDown(mouse_down);
	hinput_AddMouseDragged(mouse_down);
	hinput_AddKeyUp(key_up);
	hgui_AddSimpleElem(0.1,0,0.09,0.04,"EXIT",exit,0);
	hgui_AddSimpleElem(0.2,0,0.09,0.04,"WATER",Button,'W');
	hgui_AddSimpleElem(0.3,0,0.09,0.04,"FOREST",Button,'F');
	hgui_AddSimpleElem(0.4,0,0.09,0.04,"STREET",Button,'S');
	hgui_AddSimpleElem(0.5,0,0.09,0.04,"GRASS",Button,'G');
	hgui_AddSimpleElem(0.6,0,0.09,0.04,"ROCK",Button,'R');
	hgui_AddSimpleElem(0.7,0,0.09,0.04,"HOUSE",Button,'H');
	hgui_AddSimpleElem(0.8,0,0.09,0.04,"DEBUG",Button,'D');
}

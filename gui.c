#include "gui.h"
#include "Cell.h"
#include "Game.h"
#include "Draw.h"
#include "Client.h"

#define maxplayers 100
enum{none,leftup,rightdown,copy,paste} mouse_operation[maxplayers];
int lux[maxplayers],luy[maxplayers],rdx[maxplayers],rdy[maxplayers],sx[maxplayers],sy[maxplayers],type[maxplayers],hasbase[maxplayers],*data[maxplayers];
void gui_mouse_down(int player,EventArgs *e)
{															
	int i=e->mx,j=e->my,x,y; 
	if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))			//get world coordinates of cursor and check if it is in automat range
	{
		if(mouse_operation[player]==none)
		{
			if(type[player]==BASE && !hasbase[player])
			{
				hasbase[player]=1; 
				SetCell(i,j,Cell,state,HOUSE);						//set the cell to the selected type on left mouse
				SetCell(i,j,Cell,person,worker);					//also place someone
			}
			else
			if(type[player]!=BASE)
			{
				SetCell(i,j,Cell,command,type[player]);
				SetCell(i,j,Cell,has_command,1);
			}
		}
		if(mouse_operation[player]==leftup)
		{
			lux[player]=i; luy[player]=j;
		}
		if(mouse_operation[player]==rightdown)
		{
			rdx[player]=i; rdy[player]=j;
		}
		if(mouse_operation[player]==paste)
		{
			if(data[player]!=NULL && i>0 && j>0 && i+sx[player]<automat->n-1 && j+sy[player]<automat->n-1 && sx[player]>0 && sy[player]>0)
			{
				for(x=0;x<sx[player];x++)
				{
					for(y=0;y<sy[player];y++)
					{
						if(data[player][x*sy[player]+y]!=GetCell(i+x,j+y,Cell,state))
						{
							SetCell(i+x,j+y,Cell,has_command,1);
							SetCell(i+x,j+y,Cell,command,data[player][x*sy[player]+y]);
						}
					}
				}	
			}
		}
	}			
}
void gui_key_up(int player,EventArgs *e) 
{
	if(e->mk>='A' && e->mk<='Z')
		mouse_operation[player]=none;
	if(e->mk=='W'){ type[player]=WATER; }
	if(e->mk=='R'){ type[player]=ROCK; }
	if(e->mk=='H'){ type[player]=HOUSE; }
	if(e->mk=='F'){ type[player]=FOREST; }
	if(e->mk=='G'){ type[player]=GRASS; }
	if(e->mk=='S'){ type[player]=STREET; }
	if(e->mk=='B'){ type[player]=BASE; }
	if(e->mk=='U'){ type[player]=TERRAFORM_UP; }
	if(e->mk=='D'){ type[player]=TERRAFORM_DOWN; }
	if(e->mk=='Y'){ Draw_Set_DebugDraw(!Draw_Get_DebugDraw()); }
	if(e->mk=='P'){ mouse_operation[player]=paste; }
}
void gui_button(int player,int i)
{
	int x,y;
	if(i==none || i==leftup || i==rightdown || i==copy || i==paste)
	{
		mouse_operation[player]=i;
		if(mouse_operation[player]==copy)
		{	
			sx[player]=rdx[player]-lux[player]; sy[player]=luy[player]-rdy[player];
			if(data[player]!=NULL)
				free(data[player]);
			data[player]=(int*)calloc(1,sx[player]*sy[player]*sizeof(int));
			for(x=0;x<sx[player];x++)
			{
				for(y=0;y<sy[player];y++)
				{
					data[player][x*sy[player]+y]=GetCell(x+lux[player],y+rdy[player],Cell,state);
				}
			}
		}
	}
	else
	{
		if(client_client()==player)
		{
			client_send_key_up(&(EventArgs){0,0,i});
		}
	}
}
void gui_Init()
{
	int client=0,i;
	hinput_AddMouseDown(client_send_mouse_down);
	hinput_AddMouseDragged(client_send_mouse_down);
	hinput_AddKeyUp(client_send_key_up);
	hgui_AddSimpleElem(0.0,0,0.09,0.04,"EXIT",exit,0);
	hgui_AddSimpleElem(0.1,0,0.09,0.04,"WATER",client_send_button,'W');
	hgui_AddSimpleElem(0.2,0,0.09,0.04,"FOREST",client_send_button,'F');
	hgui_AddSimpleElem(0.3,0,0.09,0.04,"STREET",client_send_button,'S');
	hgui_AddSimpleElem(0.4,0,0.09,0.04,"GRASS",client_send_button,'G');
	hgui_AddSimpleElem(0.5,0,0.09,0.04,"ROCK",client_send_button,'R');
	hgui_AddSimpleElem(0.6,0,0.09,0.04,"HOUSE",client_send_button,'H');
	hgui_AddSimpleElem(0.7,0,0.09,0.04,"DEBUG",client_send_button,'D');
	hgui_AddSimpleElem(0.8,0,0.09,0.04,"BASE",client_send_button,'B');
	hgui_AddSimpleElem(0.0,0.96,0.09,0.04,"LEFTUP",client_send_button,leftup);
	hgui_AddSimpleElem(0.1,0.96,0.09,0.04,"RIDOWN",client_send_button,rightdown);
	hgui_AddSimpleElem(0.2,0.96,0.09,0.04,"COPY",client_send_button,copy);
	hgui_AddSimpleElem(0.3,0.96,0.09,0.04,"PASTE",client_send_button,paste);
	hgui_AddSimpleElem(0.4,0.96,0.18,0.04,"TERRAFORM UP",client_send_button,'U');
	hgui_AddSimpleElem(0.59,0.96,0.18,0.04,"TERRAFORM DOWN",client_send_button,'D');
	printf("enter desired player id integer:");
	scanf ("%d",&client);
	set_clientid(client);
	if(!SINGLEPLAYER)
		Thread_NEW(client_thread,NULL);
	for(i=0;i<maxplayers;i++)
	{
		type[i]=BASE;  //init selected types to base
	}
}

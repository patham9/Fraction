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
            #ifdef DIRECT_PLACEMENT
            //terraforming support in this mode cause its cool :)
            if(type[player]==TERRAFORM_DOWN)
            {
                SetCell(i,j,Cell,height,GetCell(i,j,Cell,height)+0.1);
                Draw_Set_HeightmapChanged();
                return;
            }
            if(type[player]==TERRAFORM_UP)
            {
                SetCell(i,j,Cell,height,GetCell(i,j,Cell,height)-0.1);
                Draw_Set_HeightmapChanged();
                return;
            }
            #endif
			#ifndef DIRECT_PLACEMENT
			if(type[player]==BASE && !hasbase[player])
            #endif
            #ifdef DIRECT_PLACEMENT
            if(type[player]==BASE)
            #endif
			{
				hasbase[player]=1; 
				SetCell(i,j,Cell,state,HOUSE);						//set the cell to the selected type on left mouse
				SetCell(i,j,Cell,person,worker);					//also place someone
			}
			else
			if(type[player]!=BASE)
			{
                #ifdef DIRECT_PLACEMENT
                SetCell(i,j,Cell,wavefront,1);
                SetCell(i,j,Cell,state,type[player]);
                #endif
                #ifndef DIRECT_PLACEMENT
				SetCell(i,j,Cell,command,type[player]);
				SetCell(i,j,Cell,has_command,1);
                #endif
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
							#ifndef DIRECT_PLACEMENT
							SetCell(i+x,j+y,Cell,has_command,1);
							SetCell(i+x,j+y,Cell,command,data[player][x*sy[player]+y]);
                            #endif
                            #ifdef DIRECT_PLACEMENT
                            SetCell(i+x,j+y,Cell,wavefront,1);
							SetCell(i+x,j+y,Cell,state,data[player][x*sy[player]+y]);
                            #endif
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
    if(e->mk=='V'){ type[player]=AGENT; } 
    if(e->mk=='M'){ type[player]=OPENROCK; }
    if(e->mk=='I'){ type[player]=OFFCURRENT; }
    if(e->mk=='E'){ type[player]=SWITCH; }
    if(e->mk=='Q'){ type[player]=OFFSWITCH; }
    if(e->mk=='N'){ type[player]=NEG; }
    if(e->mk=='A'){ type[player]=AND; }
    if(e->mk=='X'){ type[player]=XOR; }
    if(e->mk=='O'){ type[player]=OR; }
    if(e->mk=='C'){ type[player]=CURRENT; }
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
void gui_buttons_draw()
{
    hrend_SelectColor(1,1,1,1);
    //hrend_DrawObj(0.1,0,0,0.03,1,COMMAND);
    hgui_RENDER();
}
void gui_Init()
{
	int client=0,i;
	hinput_AddMouseDown(client_send_mouse_down);
	hinput_AddMouseDragged(client_send_mouse_down);
	hinput_AddKeyUp(client_send_key_up);
	hgui_AddSimpleElem(0.0,0,0.09,0.04,"EXIT",exit,0,NULL);
	hgui_AddSimpleElem(0.1,0,0.09,0.04,"WATER",client_send_button,'W',WATER);
	hgui_AddSimpleElem(0.2,0,0.09,0.04,"FOREST",client_send_button,'F',FOREST);
	hgui_AddSimpleElem(0.3,0,0.09,0.04,"STREET",client_send_button,'S',STREET);
	hgui_AddSimpleElem(0.4,0,0.09,0.04,"GRASS",client_send_button,'G',GRASS);
	hgui_AddSimpleElem(0.5,0,0.09,0.04,"ROCK",client_send_button,'R',ROCK);
	hgui_AddSimpleElem(0.6,0,0.09,0.04,"HOUSE",client_send_button,'H',HOUSE);
	hgui_AddSimpleElem(0.7,0,0.09,0.04,"DEBUG",client_send_button,'D',NULL);
	hgui_AddSimpleElem(0.8,0,0.09,0.04,"BASE",client_send_button,'B',HOUSE);
    
	hgui_AddSimpleElem(0.0,0.96,0.09,0.04,"LEFTUP",client_send_button,leftup,NULL);
	hgui_AddSimpleElem(0.1,0.96,0.09,0.04,"RIDOWN",client_send_button,rightdown,NULL);
	hgui_AddSimpleElem(0.2,0.96,0.09,0.04,"COPY",client_send_button,copy,NULL);
	hgui_AddSimpleElem(0.3,0.96,0.09,0.04,"PASTE",client_send_button,paste,NULL);
	hgui_AddSimpleElem(0.4,0.96,0.18,0.04,"TERRA UP",client_send_button,'U',NULL);
	hgui_AddSimpleElem(0.59,0.96,0.18,0.04,"TERRA DOWN",client_send_button,'D',NULL);
    hgui_AddSimpleElem(0.78,0.96,0.09,0.04,"XOR",client_send_button,'X',XOR);
	hgui_AddSimpleElem(0.88,0.96,0.09,0.04,"OR",client_send_button,'O',OR);
    
    hgui_AddSimpleElem(0.0,0.91,0.09,0.04,"AGENT",client_send_button,'V',AGENT);
	hgui_AddSimpleElem(0.1,0.91,0.09,0.04,"WIRE1",client_send_button,'C',CURRENT);
	hgui_AddSimpleElem(0.2,0.91,0.09,0.04,"WIREO",client_send_button,'I',OFFCURRENT);
	hgui_AddSimpleElem(0.3,0.91,0.09,0.04,"ON",client_send_button,'E',SWITCH);
	hgui_AddSimpleElem(0.4,0.91,0.18,0.04,"OFF",client_send_button,'Q',OFFSWITCH);
    hgui_AddSimpleElem(0.78,0.91,0.09,0.04,"NEG",client_send_button,'N',NEG);
	hgui_AddSimpleElem(0.88,0.91,0.09,0.04,"AND",client_send_button,'A',AND);
    hrend_SetGUIRenderRoutine(gui_buttons_draw);

    if(!SINGLEPLAYER)
    {
        printf("enter desired player id integer:");
        scanf ("%d",&client);
    }
	set_clientid(client);
	if(!SINGLEPLAYER)
		Thread_NEW(client_thread,NULL);
	for(i=0;i<maxplayers;i++)
	{
		type[i]=BASE;  //init selected types to base
	}
}

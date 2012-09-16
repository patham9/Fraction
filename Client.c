#include "Client.h"
#include "stdio.h"
#include "gui.h"

// SOCKET
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
// SOCKET END

int client=0,player=0,sockfd=0,MOUSE=1,KEY=2,BUTTON=3,ID=4,step;	//global constants once inited
void client_get_clientid(){}
int client_connect()
{
    struct sockaddr_in serv_addr;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) return -1;
    memset(&serv_addr,'0',sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);
    if(inet_pton(AF_INET,IP,&serv_addr.sin_addr)<=0) return -1;
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) return -1;
    client_get_clientid();
    player=client; 														//every player plays with own people currently
    return sockfd;
}
void client_send_mouse_down(EventArgs *e)
{
	int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5);
	if(e->mk!=1)
		return;
	if(SINGLEPLAYER)
	{
		gui_mouse_down(client,&(EventArgs){i,j,e->mk});
		return;
	}
	write(sockfd,&MOUSE,sizeof(int));
	write(sockfd,&client,sizeof(int));
	write(sockfd,&(e->mk),sizeof(int));									
	write(sockfd,&i,sizeof(int));
	write(sockfd,&j,sizeof(int));
}
void client_send_key_up(EventArgs *e) 
{
	if(!(e->mk>='A' && e->mk<='Z'))
		return;
	if(SINGLEPLAYER)
	{
		gui_key_up(client,e);
		return;
	}
	write(sockfd,&KEY,sizeof(int));
	write(sockfd,&client,sizeof(int));
	write(sockfd,&(e->mk),sizeof(int));									//don't want to care about send length yet, this low amount of bytes can go into a single IP packet anyway
	write(sockfd,&(e->mk),sizeof(int));
	write(sockfd,&(e->mk),sizeof(int));
}
void client_send_button(int i)
{	
	if(SINGLEPLAYER)
	{
		gui_button(client,i);
	}
	write(sockfd,&BUTTON,sizeof(int));
	write(sockfd,&client,sizeof(int));
	write(sockfd,&i,sizeof(int));
	write(sockfd,&i,sizeof(int));
	write(sockfd,&i,sizeof(int));
}
int client_client()
{
	return client;
}
int client_player()
{
	return player;
}
int get_step()
{
	return step;
}
void *client_thread(void *arg)
{
    int width,height;
    client_connect();
    while(1)
    {
		int req_id,req_client,req_mx,req_my,req_mk;						//receive and handle events here
		read(sockfd,&req_id,sizeof(int));
		read(sockfd,&req_client,sizeof(int));
		read(sockfd,&req_mk,sizeof(int));
		read(sockfd,&req_mx,sizeof(int));
		read(sockfd,&req_my,sizeof(int));
		printf("req id %d \n",req_id);
		if(req_id==MOUSE)								
			gui_mouse_down(req_client,&(EventArgs){req_mx,req_my,req_mk});
		else
		if(req_id==KEY)
			gui_key_up(req_client,&(EventArgs){0,0,req_mk});
		else
		if(req_id==BUTTON)
			gui_button(req_client,req_mk);
		else
		if(req_id!=ID)
		{
			step++;
			continue;
		}
		Wait(0.001);
    }
}

void Men_SetNextTarget(Men *men,int state, Cell ***readcells)
{
	int i,j;
	int besti=0,bestj=0;
	float bestdist=99999;
	for(i=0;i<worldsize;i++)
	{
		for(j=0;j<worldsize;j++)
		{
			if(readcells[i][j]->state==state)
			{
				float dist=sqrt(pow(men->x-i,2)+pow(men->y-j,2));
				if(dist<bestdist)
				{
					bestdist=dist;
					besti=i;
					bestj=j;
				}
			}
		}
	}
	men->targetx=besti;
	men->targety=bestj;
}

void Men_Execute(Men* men,Cell ***readcells)
{
	int x=(int)(men->x+0.5);
	int y=(int)(men->y+0.5);
	int tx=(int)(men->targetx);
	int ty=(int)(men->targety);
	if(readcells[x][y]->state==ROCK || readcells[x][y]->state==WATER)
	{
		men->dead=1;
		return;
	}
	if(men->workstate==0 && readcells[tx][ty]->state!=FOREST)			//he has workstate 0 so he is a wood feller and needs to find wood
	{
		Men_SetNextTarget(men,FOREST,readcells);
	}
	if(men->workstate==1 && readcells[tx][ty]->state!=FOREST) 			//he has workstate 1 so he is a wood feller who found wood so return
	{
		Men_SetNextTarget(men,CITY,readcells);
	}
	if(men->workstate==0 && readcells[x][y]->state==FOREST)				//he was wood feller and found wood
	{
		Men_SetNextTarget(men,CITY,readcells);
		men->wood=100;
		men->workstate+=1;
		SetCell(x,y,Cell,state,GRASS);
	}
	if(men->workstate==1 && readcells[x][y]->state==CITY) 				//he was wood feller who found wood and returned home
	{
		SetCell(x,y,Cell,wood,GetCell(x,y,Cell,wood)+men->wood);
		men->wood=0;
		men->dead=1;
	}
	else
	{
		float speed=0.01;
		float dx=men->targetx-men->x;
		float dy=men->targety-men->y;
		float a=atan2(dy,dx);
		men->x+=speed*cos(a);
		men->y+=speed*sin(a);
	}
}

void mouse_down(EventArgs *e)
{																		//get world coordinates of cursor and check if it is in the automats range
	int i=(int)(hnav_MouseToWorldCoordX(e->mx)+0.5),j=(int)(hnav_MouseToWorldCoordY(e->my)+0.5); 
	if(i>=0 && j>=0 && i<(automat->n) && j<(automat->n))
	{
		if(e->mk==1)
		{
			SetCell(i,j,Cell,state,type); SetCell(i,j,Cell,rootwater,NULL);	//set the cell to the selected type on left mouse
		}																
	}
}

void key_up(EventArgs*e) 
{
	if (e->mk=='X')
	{													
		running = 0;													// running will end the main loop of the thread
		glfwWaitThread(thread, 0);										// then we wait and exit normally
		exit(0);
	}
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
	if(e->mk=='S')														
	{
		type=STREET;													//select the types for the placement of objects with keys
	}
}

void Automat_Thread()
{
	while(running)
	{
		Hauto_OBJ_Exec(automat);										//update automat state and 
#if RENDERMODE!=0														
		btoGPU=1;
#endif
		Wait(0.001);													//wait a bit to not consume max CPU core time
	}					
}

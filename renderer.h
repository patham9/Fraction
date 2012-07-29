float *toGPU; //2D array of state, lastchange, height, wateramount
int btoGPU=0, Initial=1;
void draw()
{
	int i,j,k=0;
#if RENDERMODE!=1														
	for(j=automat->n-1;j>=0;j--)										//iterate through for(i=automat->n-1;i>0;i--)	
	{
		for(i=automat->n-1;i>=0;i--)									//the grid
		{	
#endif
#if RENDERMODE!=1
			Cell *c=((Cell*)automat->readCells[i][j]);	
#if RENDERMODE!=0
			toGPU[k]=1.0/(float)c->state; k++;
			toGPU[k]=1.0/(0.1+(float)c->lastchange); k++;
			toGPU[k]=1.0/(0.1+(float)c->height); k++;
			toGPU[k]=1.0/(0.1+(float)c->wateramount); k++;
#endif
#if RENDERMODE==0
			toGPU[k]=0.5+((Cell*)automat->readCells[i][j])->height/20.0; k++;	//use texture for rendering instead in mode 0
			toGPU[k]=0.6+((Cell*)automat->readCells[i][j])->height/20.0; k++;
			toGPU[k]=0.2+((Cell*)automat->readCells[i][j])->height/20.0+((Cell*)automat->readCells[i][j])->wateramount/5.0; k++;
			toGPU[k]=1.0; k++;
#endif
		}
	}
#endif
#if RENDERMODE!=1
#if RENDERMODE!=0
	glUniform1f(shader_t,(float)glfwGetTime());							//set shader time
	glUniform1f(shader_px,hnav_MouseToWorldCoordX(hrend.width/2));
	glUniform1f(shader_py,hnav_MouseToWorldCoordY(hrend.height/2));					
	glUniform1f(shader_zoom,hnav.zoom);
#endif
	glActiveTexture(GL_TEXTURE0);										//give data to GPU
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GPUTex);
	if(btoGPU || Initial)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, worldsize, worldsize, 0, GL_RGBA, GL_FLOAT, toGPU);
#if RENDERMODE!=0
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	//we need the values direct
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
#endif
#if RENDERMODE==0
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//we need the values direct
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
#endif
		btoGPU=0; Initial=0;
	}
#if RENDERMODE>=2
	glUseProgram(shader);												//use pixel (and vertex) shader
	glActiveTexture(GL_TEXTURE1);										//give textures to GPU
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GRASS);
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,CITY);
	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,FOREST);
	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ROCK);
	glActiveTexture(GL_TEXTURE5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,STREET);
	glActiveTexture(GL_TEXTURE6);
	glEnable(GL_TEXTURE_2D);
	hrend_DrawObj(worldsize/2-0.25,worldsize/2-0.25,0,worldsize/2,1,WATER);
#endif
#endif
#if RENDERMODE==0 || RENDERMODE==1
#if RENDERMODE==0
	hrend_SelectColor(0.45, 0.59, 0.31,1);
	hrend_DrawObj(worldsize/2-0.25,worldsize/2-0.25,0,worldsize/2,1,GPUTex);
#endif
	for(i=1;i<automat->n-1;i++)											//iterate through
	{
		for(j=1;j<automat->n-1;j++)										//the grid
		{	
			Cell ***readcells=(Cell***)automat->readCells;
			Cell *c=readcells[i][j];
			
#if RENDERMODE==0
			if(c->state!=GRASS)
#endif			
			{															//draw cell only if camera sees, wouldn't make sense else
				if(abs(hnav_MouseToWorldCoordX(hrend.width/2)-i)<hnav_MouseToWorldCoordX(hrend.width/2)-hnav_MouseToWorldCoordX(0) 	
				&& abs(hnav_MouseToWorldCoordY(hrend.height/2)-j)<hnav_MouseToWorldCoordY(0)-hnav_MouseToWorldCoordY(hrend.height/2))
				{															//select color and draw
					int grass_around=NeighborsValue(op_plus,being_a,GRASS);
					hrend_SelectColor(0.5+c->height/20.0,0.6+c->height/20.0,0.2+c->height/20.0+c->wateramount/5.0,1);
					if(c->state==WATER && grass_around>=3)
					{
						if(grass_around>=8)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_A);
						}
						else
						if(readcells[i+1][j]->state==GRASS && readcells[i][j+1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_RT);
						}
						else
						if(readcells[i-1][j]->state==GRASS && readcells[i][j-1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_LD);
						}
						else
						if(readcells[i+1][j]->state==GRASS && readcells[i][j-1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_RD);
						}
						else
						if(readcells[i-1][j]->state==GRASS && readcells[i][j+1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_LT);
						}
						else
						if(readcells[i+1][j]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_R);
						}
						else
						if(readcells[i-1][j]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_L);
						}
						else
						if(readcells[i][j+1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_T);
						}
						else
						if(readcells[i][j-1]->state==GRASS)
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS_D);
						}
						else
						{
							hrend_DrawObj(i,j,0,0.5,1,GRASS);
						}
					}
					else
						hrend_DrawObj(i,j,0,0.5,1,c->state);
				}
			}
		}
	}
#endif
#if RENDERMODE>=2
	glUseProgram(0);													//back to fixed pipeline
	glActiveTexture(GL_TEXTURE0);										//set texture0
#endif
	for(i=0;i<meni;i++)
	{
		if(men[i]->dead==0)
		{
			Men_Execute(men[i],automat->readCells);
			hrend_DrawObj(men[i]->x,men[i]->y,0,0.5,1,MEN);
		}
	}
	Wait(0.001);
}

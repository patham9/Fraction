void GenerateNature()
{
	int i,j;
	for(i=2;i<worldsize-2;i++)											//iterate through
	{
		for(j=2;j<worldsize-2;j++)										//the grid
		{	
			if(drnd()>0.999940)											
			{
				SetCell(i,j,Cell,state,WATER);							//maybe place some water
			}
			if(drnd()>0.9990)
			{
				SetCell(i,j,Cell,state,FOREST);							//maybe some forest
				SetCell(i+1,j,Cell,state,FOREST);
				SetCell(i,j+1,Cell,state,FOREST);
				SetCell(i-1,j,Cell,state,FOREST);
			}
		}
	}
}

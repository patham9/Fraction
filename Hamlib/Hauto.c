#include "Hauto.h"

Hauto_OBJ *Hauto_OBJ_NEW(int nstatistics,int n,void (*Exec)(int t,int i,int j,void *writeme,void* readme,void* left,void* right,void* up,void* down,
                               void* left_up,void* left_down,void* right_up,void* right_down),void* (*New_Cell)(int,int))
{
	int i,j;
    Hauto_OBJ *ret=(Hauto_OBJ*) malloc(sizeof(Hauto_OBJ));
    ret->t=0;
    ret->nstatistics=nstatistics;
    ret->n=n;
    ret->Exec=Exec;
    ret->readCellStatistics=(int*) calloc(nstatistics,sizeof(int));
    ret->writeCellStatistics=(int*) calloc(nstatistics,sizeof(int));
    ret->readCells=(void***) malloc(n*sizeof(void**));
    ret->writeCells=(void***) malloc(n*sizeof(void**));
    for(i=0;i<n;i++)
    {
		ret->readCells[i]=(void**) malloc(n*sizeof(void*));
		ret->writeCells[i]=(void**) malloc(n*sizeof(void*));
		for(j=0;j<n;j++)
		{
			ret->readCells[i][j]=New_Cell(i,j);
			ret->writeCells[i][j]=New_Cell(i,j);
		}
	}
    return ret;
}
void Hauto_OBJ_Exec(Hauto_OBJ *aut)
{
	int i,j,*temp;
	aut->t++;
	for(i=1;i<aut->n-1;i++)
	{
		for(j=1;j<aut->n-1;j++)
		{
			aut->Exec(aut->t,i,j,aut->writeCells[i][j],aut->readCells[i][j],aut->readCells[i-1][j],aut->readCells[i+1][j],aut->readCells[i][j+1],aut->readCells[i][j-1],
			          aut->readCells[i-1][j+1],aut->readCells[i-1][j-1],aut->readCells[i+1][j+1],aut->readCells[i+1][j-1],aut->readCells);
		}
	}
	//change write to read and read to write
	void ***temp2=aut->readCells;
	aut->readCells=aut->writeCells;
	aut->writeCells=temp2;
	temp=aut->readCellStatistics;
	aut->readCellStatistics=aut->writeCellStatistics;
	aut->writeCellStatistics=temp;
	for(i=0;i<aut->nstatistics;i++)
	{
		aut->writeCellStatistics[i]=0;
	}
}
float Hauto_OBJ_NeighborsValue(float (*Op)(float, float),int i,int j,void ***readCells,float (*Neighbor_Value)(void*,void*),void* data)
{
	return Op(Op(Op(Op(Op(Op(Op(Neighbor_Value(readCells[i+1][j],data),Neighbor_Value(readCells[i-1][j],data)),Neighbor_Value(readCells[i+1][j+1],data)),
	Neighbor_Value(readCells[i-1][j-1],data)),Neighbor_Value(readCells[i][j+1],data)),Neighbor_Value(readCells[i][j-1],data)),
	Neighbor_Value(readCells[i-1][j+1],data)),Neighbor_Value(readCells[i+1][j-1],data));
}
void *Hauto_OBJ_FirstNeighbor(int i,int j,void ***readCells,float (*Neighbor_Value)(void*,void*),void* data)
{
	int k,l;
	for(k=i-1;k<=i+1;k++)
	{
		for(l=j-1;l<=j+1;l++)
		{
			if(!(k==i && j==l))
			{
				if(Neighbor_Value(readCells[k][l],data))
				{
					return readCells[k][l];
				}
			}
		}
	}
	return NULL;
}
///////////STANDARD-MODULE-FUNCTIONS///////////
void hauto_INIT()
{
		hauto.mode=MODE;
		hauto.init=1;
}
void hauto_UPDATE(){if(hauto.init){}}
///////////////////////////////////////////////


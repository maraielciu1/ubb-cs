#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{	
	int** m;
	int r,c;
	
	FILE* f;
	f=fopen(argv[1],"r");
	fscanf(f,"%d %d",&r,&c);
	printf("%d %d\n",r,c);
	m=(int**)malloc(r*sizeof(int*));

	for(int i=0;i<r;i++)
	{
		m[i]=(int*)malloc(c*sizeof(int));
		for(int j=0;j<c;j++)
			fscanf(f,"%d",&m[i][j]);
	}	
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
			printf("%d ",m[i][j]);
		printf("\n");
	}

	for(int i=0;i<r;i++)
		free(m[i]);
	free(m);

	fclose(f);
	return 0;
}

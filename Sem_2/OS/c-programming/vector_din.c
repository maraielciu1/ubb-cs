#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{

	int n;
	FILE* f;
	f=fopen(argv[1],"r");
	int* v;
	fscanf(f,"%d",&n);
	printf("%d\n",n);
	v=(int*)malloc(n*sizeof(int));
	for(int i=0;i<n;i++)
	{	fscanf(f,"%d",&v[i]);
		printf("%d ",v[i]);
	}
	free(v);
	fclose(f);
	return 0;
}


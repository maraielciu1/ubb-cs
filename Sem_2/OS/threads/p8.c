#include <pthread.h>
#include <stdio.h>

int count=0;

void* f(void* a){
    int i;
    for(i=0;i<*(int*)a;i++)
        count++;
    return NULL;
}

int main(int argc, char** argv){
    pthread_t t[10];
    int i,n=10;

    if(argc>1)
        sscanf(argv[1],"%d",&n);

    for(int i=0;i<10;i++){
        pthread_create(&t[i],NULL,f,&n);
    }

    for(i=0;i<10;i++)
        pthread_join(t[i],NULL);

    printf("%d\n",count);
    return 0;
}

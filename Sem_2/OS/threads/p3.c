#include <pthread.h>
#include <stdio.h>

int n=1;

void* f(void* a){
    int i;
    for(i=0;i<n;i++)
        printf("%s\n",(char*)a);
    return NULL;
}

int main(int argc, char** argv){
    pthread_t ta,tb;
    int i;
    
    if(argc>1)
        sscanf(argv[1],"%d",&n);
    
    pthread_create(&ta,NULL,f,"fa");
    pthread_create(&tb,NULL,f,"fb");

    for(i=0;i<n;i++)
        printf("main\n");

    pthread_join(ta,NULL);
    pthread_join(tb,NULL);
    return 0;
}

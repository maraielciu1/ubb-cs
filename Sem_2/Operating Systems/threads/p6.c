#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* f(void* a){
    printf("%d\n", *((int*)a));
    free(a);
    return NULL;
}

int main(int argc, char** argv){
    int i;
    int* p;
    pthread_t t[10];
    for(i=0;i<10;i++){
        p=(int*)malloc(sizeof(int));
        *p=i;
        pthread_create(&t[i],NULL,f,p);
    }
    for(i=0;i<10;i++)
        pthread_join(t[i],NULL);

    return 0;
}

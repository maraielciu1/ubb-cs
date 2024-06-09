#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "barrier.h"
int sum=0;
pthread_barrier_t bar;
pthread_mutex_t mtx;

void* f(void* a){
    int id=*((int*)a);
    int n1=rand()%11;
    int n2=rand()%11;
    int n3=rand()%11;
    printf("thread %d generated: %d %d %d\n", id,n1,n2,n3);
    pthread_barrier_wait(&bar);
    // each thread has reached here now
    pthread_mutex_lock(&mtx);
    if(n1%2==0)
        sum-=n1;
    else
        sum+=n1;
    if(n2%2==0)
        sum-=n2;
    else
        sum+=n2;
    if(n3%2==0)
        sum-=n3;
    else
        sum+=n3;
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main(int argc, char** argv){
    int n=atoi(argv[1]);
    pthread_t th[n];
    int a[n];
    pthread_mutex_init(&mtx,NULL);
    pthread_barrier_init(&bar,NULL,n);
    for(int i=0;i<n;i++)
    {
        a[i]=i;
        pthread_create(&th[i],NULL,f,&a[i]);

    }
    for(int i=0;i<n;i++)
        pthread_join(th[i],NULL);
    pthread_mutex_destroy(&mtx);
    pthread_barrier_destroy(&bar);
    printf("sum=%d",sum);
    return 0;
}

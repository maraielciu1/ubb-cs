// suma a 2 matrici
#include <stdio.h>
#include <pthread.h>
#include "pthread_barrier.h"

int* current_col;
pthread_barrier_t bar;
pthread_mutex_t mtx;

int n;
int a[100][100];
int b[100][100];
int c[100][100];

void* f(void* arg){
    int row=*((int*)arg);
    while(1){
        if(row>=n)
            break;
        int current=*current_col;
        c[row][current]=(a[row][current]+b[row][current]);
        pthread_barrier_wait(&bar);
        pthread_mutex_lock(&mtx);
        *(current_col)+=1;
        pthread_mutex_unlock(&mtx);
        pthread_barrier_wait(&bar);
    }
    return NULL;
}

int main(int argc, char** argv){
    printf("dimensiunea=");
    scanf("%d",&n);
    pthread_barrier_init(&bar,NULL,n);
    pthread_mutex_init(&mtx,NULL);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            scanf("%d",&a[i][j]);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            scanf("%d",&b[i][j]);
    int arr[n];
    pthread_t th[n];
    for(int i=0;i<n;i++)
    {
        arr[i]=i;
        pthread_create(&th[i],NULL,f,&arr[i]);
    }
    for(int i=0;i<n;i++)
        pthread_join(th[i],NULL);
    return 0;
}

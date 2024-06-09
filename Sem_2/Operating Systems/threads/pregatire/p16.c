//Write a C program that receives integers as command line argument. The program will keep a frequency vector for all digits. The program will create a thread for each argument that counts the number of occurences of each digit and adds the result to the frequency vector. Use efficient synchronization.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mtx;
int freq[10];

void* f(void* a){
    int nr=*((int*)a);
    if(nr==0){
        pthread_mutex_lock(&mtx);
        freq[0]++;
        pthread_mutex_unlock(&mtx);
    }
    while(nr){
        int cf=nr%10;
        pthread_mutex_lock(&mtx);
        freq[cf]++;
        pthread_mutex_unlock(&mtx);
        nr/=10;
    }
    return NULL;
}

int main(int argc, char** argv){
    pthread_t th[argc-1];
    pthread_mutex_init(&mtx,NULL);
    for(int i=0;i<argc-1;i++)
    {
        int nr=atoi(argv[i+1]);
        pthread_create(&th[i],NULL,f,(void*)&nr);
    }

    for(int i=0;i<argc-1;i++)
        pthread_join(th[i],NULL);
    for(int i=0;i<10;i++)
        printf("%d ",freq[i]);
    pthread_mutex_destroy(&mtx);
    return 0;
}

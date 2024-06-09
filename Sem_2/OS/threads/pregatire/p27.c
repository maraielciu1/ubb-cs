//Write a C program that takes two numbers, N and M, as arguments from the command line. The program creates N "generator" threads that generate random lowercase letters and append them to a string with 128 positions. The program will create an additional "printer" thread that that waits until all the positions of the string are filled, at which point it prints the string and clears it. The N "generator" threads must generate a total of M such strings and the "printer" thread prints each one as soon as it gets to length 128.
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct{
    char *str;
    pthread_cond_t *c;
    pthread_mutex_t* mtx;
    int n;
    int* pos;
    int* m;
}data;

void* fill(void* a){
    data d=*((data*)a);
    while(1){
        pthread_mutex_lock(d.mtx);
        int nr=rand()%26;
        char l=nr+'a';
        while(*d.pos==d.n){
            pthread_cond_broadcast(d.c);
            pthread_cond_wait(d.c,d.mtx);
        }
        if(*d.m==0){
            pthread_mutex_unlock(d.mtx);
            break;
        }
        d.str[*d.pos]=l;
        *(d.pos)+=1;
        pthread_mutex_unlock(d.mtx);

    }
    return NULL;
}

void* empty(void* a){
    data d=*((data*)a);
    while(1){
        pthread_mutex_lock(d.mtx);
        while(*d.pos!=d.n){
            pthread_cond_broadcast(d.c);
            pthread_cond_wait(d.c,d.mtx);
        }
        printf("%s\n",d.str);
        memset(d.str,0,sizeof(char)*128);
        *(d.pos)=0;
        *(d.m)-=1;
        if(*d.m==0){
            pthread_mutex_unlock(d.mtx);
            break;
        }
        pthread_mutex_unlock(d.mtx);
    }
    return NULL;
}

int main(int argc, char** argv){
    int n=128;
    pthread_mutex_t* mtx=malloc(sizeof(pthread_mutex_t));
    pthread_cond_t* c=malloc(sizeof(pthread_mutex_t));
    char* str=malloc(129);
    int N=atoi(argv[1]);
    int* m=malloc(sizeof(int));
    *(m)=atoi(argv[2]);
    data* args=malloc(sizeof(data)*(N+1));
    pthread_t* th=malloc(sizeof(pthread_t)*(N+1));

    pthread_mutex_init(mtx,NULL);
    pthread_cond_init(c,NULL);
    int* pos=malloc(sizeof(int));
    *(pos)=0;

    for(int i=0;i<N;i++)
    {
        args[i].str=str;
        args[i].c=c;
        args[i].mtx=mtx;
        args[i].n=n;
        args[i].m=m;
        args[i].pos=pos;
        pthread_create(&th[i],NULL,fill,&args[i]);
    }
    args[N].str=str;
    args[N].c=c;
    args[N].mtx=mtx;
    args[N].n=n;
    args[N].m=m;
    args[N].pos=pos;
    pthread_create(&th[N],NULL,fill,&args[N]);
    for(int i=0;i<=N;i++)
        pthread_join(th[i],NULL);

    return 0;
}

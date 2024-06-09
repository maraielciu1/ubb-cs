//Write a program that receives strings of characters as command line arguments. For each string the program creates a thread which calculates the number of digits, the number of leters and the number of special characters (anything other than a letter or digit). The main program waits for the threads to terminate and prints the total results (total number of digits, letters and special characters across all the received command line arguments) and terminates. Use efficient synchronization. Do not use global variables
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

typedef struct{
    int *letters;
    int *digits;
    int *special;
    pthread_mutex_t *mutexes;
    char *str;
}data;

void* f(void* a){
    data d=*((data*)a);
    int len=strlen(d.str);
    int l=0,di=0,s=0;
    for(int i=0;i<len;i++)
    {
        if((d.str[i]>='a'&&d.str[i]<='z')||(d.str[i]>='A'&&d.str[i]<='Z'))
            l++;
        else
            if(d.str[i]>='0'&&d.str[i]<='9')
                di++;
            else
                s++;
        
    }
    if(l>0){
        pthread_mutex_lock(&d.mutexes[0]);
       *( d.letters)+=l;
        pthread_mutex_unlock(&d.mutexes[0]);
    }
    if(di>0){
        pthread_mutex_lock(&d.mutexes[1]);
        *(d.digits)+=di;
        pthread_mutex_unlock(&d.mutexes[1]);
    }
    if(s>0){
        pthread_mutex_lock(&d.mutexes[2]);
        *(d.special)+=s;
        pthread_mutex_unlock(&d.mutexes[2]);
    }
    return NULL;
}

int main(int argc, char** argv){
    int* letters=malloc(sizeof(int));
    int* digits=malloc(sizeof(int));
    int* special=malloc(sizeof(int));
    
    pthread_mutex_t* mtx=malloc(sizeof(pthread_mutex_t)*3);
    pthread_t* th=malloc(sizeof(pthread_t)*(argc-1));
    for(int i=0;i<3;i++)
        pthread_mutex_init(&mtx[i],NULL);

    *letters=0;
    *digits=0;
    *special=0;
    data* args=malloc(sizeof(data)*(argc-1));

    for(int i=0;i<(argc-1);i++){
        args[i].letters=letters;
        args[i].digits=digits;
        args[i].special=special;
        args[i].mutexes=mtx;
        args[i].str=argv[i+1];
        pthread_create(&th[i],NULL,f,(void*)&args[i]);
    }

    for(int i=0;i<argc-1;i++)
        pthread_join(th[i],NULL);

    printf("letters %d, digits %d, special %d",*letters,*digits,*special);

    for(int i=0;i<3;i++)
        pthread_mutex_destroy(&mtx[i]);
    free(letters);
    free(digits);
    free(special);
    free(args);
    free(th);
    return 0;
}

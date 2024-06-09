// numara acte vocale sunt
#include <stdio.h>
#include <pthread.h>
#include <string.h>

int total;
pthread_mutex_t mtx;

void* f(void* a){
    char* str=(char*)a;
    int sum=0;
    for(int i=0;i<strlen(str);i++)
        if(str[i]=='a'||str[i]=='A'||str[i]=='e'||str[i]=='E'||str[i]=='i'||str[i]=='I'||str[i]=='o'||str[i]=='O'||str[i]=='u'||str[i]=='U')
            sum++;
    pthread_mutex_lock(&mtx);
    total+=sum;
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(int argc, char** argv){
    pthread_mutex_init(&mtx,NULL);
    int n;
    printf("Give the number of words: ");
    scanf("%d",&n);
    pthread_t th[n];
    for(int i=0;i<n;i++)
    {
        char str[100];
        scanf("%s",str);
        pthread_create(&th[i],NULL,f,&str);
    }
    for(int i=0;i<n;i++)
        pthread_join(th[i],NULL);
    printf("%d",total);
    pthread_mutex_destroy(&mtx);
    return 0;
}

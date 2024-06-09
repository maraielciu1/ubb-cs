#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "pthread_barrier.h"

pthread_mutex_t m;
pthread_barrier_t barrier;

int a[100][100];
int b[100][100];
int c[100][100];
int i;
int n;

void* threads(void* arg)
{
        int row=*(int *)arg;
        int x,y,sum;
        while(1)
        {
                pthread_mutex_lock(&m);
                if(i>=n)
                {
                        pthread_mutex_unlock(&m);
                        break;
                }
                x=a[row][i];
                y=b[row][i];
                sum=x+y;
                c[row][i]=sum;

                pthread_mutex_unlock(&m);

                pthread_barrier_wait(&barrier);

                pthread_mutex_lock(&m);
                if(row==0)
                {
                        i++;
                }
                pthread_mutex_unlock(&m);

                pthread_barrier_wait(&barrier);
        }
        return NULL;
}

int main(int argc, char **argv)
{
        i=0;
        printf("Insert the number for the square matrix: ");
        scanf("%d",&n);

        printf("Insert the values for the first matrix:\n");
        for(int i=0;i<n;i++)
        {
                for(int j=0;j<n;j++)
                {
                        printf("Row: %d, Column: %d, Value: ",i+1,j+1);
                        scanf("%d",&a[i][j]);
                }
        }

        printf("Insert the values for the second matrix:\n");
        for(int i=0;i<n;i++)
        {
                for(int j=0;j<n;j++)
                {
                        printf("Row: %d, Column: %d, Value: ",i+1,j+1);                             			scanf("%d",&b[i][j]);
                }
        }
        pthread_t t[n];
        pthread_barrier_init(&barrier,NULL,n);
        pthread_mutex_init(&m,NULL);

        int indexes[n];
        for(int i=0;i<n;i++)
        {
                indexes[i]=i;
                pthread_create(&t[i],NULL,threads, &indexes[i]);
        }

        for(int i=0;i<n;i++)
        {
                pthread_join(t[i],NULL);
        }

        printf("The sum of matrix A and B:\n");
        for(int i=0;i<n;i++)
        {
                for(int j=0;j<n;j++)
                {
                        printf("Row: %d, Column: %d, Value: %d\n",i+1,j+1, c[i][j]);
                }
        }

        //pthread_mutex_destroy(&m);
       // pthread_barrier_destroy(&barrier);

        return 0;
}

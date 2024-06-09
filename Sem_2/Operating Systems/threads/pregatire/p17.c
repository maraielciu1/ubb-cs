#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int n;
int index = 0;
int *arr;

void* f1(void* a) {
    while (1) {
        pthread_mutex_lock(&mtx);
        while (index % 2 != 0) {
            pthread_cond_wait(&condition, &mtx);
        }
        if (index >= n) {
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mtx);
            break;
        }
        int nr = rand() % 100 * 2;
        arr[index] = nr;
        index++;
        printf("T1: ");
        for (int i = 0; i < index; i++)
            printf("%d ", arr[i]);
        printf("\n");
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&mtx);
        usleep(100); // Added to prevent tight loop and to make thread alternation visible
    }
    return NULL;
}

void* f2(void * a) {
    while (1) {
        pthread_mutex_lock(&mtx);
        while (index % 2 != 1) {
            pthread_cond_wait(&condition, &mtx);
        }
        if (index >= n) {
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&mtx);
            break;
        }
        int nr = rand() % 100 * 2 + 1;
        arr[index] = nr;
        index++;
        printf("T2: ");
        for (int i = 0; i < index; i++)
            printf("%d ", arr[i]);
        printf("\n");
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&mtx);
        usleep(100); // Added to prevent tight loop and to make thread alternation visible
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <number of elements>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        printf("Number of elements must be greater than 0\n");
        return 1;
    }

    arr = malloc(sizeof(int) * n);
    if (arr == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    pthread_t th[2];

    pthread_create(&th[0], NULL, f1, NULL);
    pthread_create(&th[1], NULL, f2, NULL);

    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);

    free(arr);
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&condition);
    return 0;
}


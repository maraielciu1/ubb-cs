#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int id, N;
    pthread_barrier_t *barrier;
    sem_t * sems;
} data;

void *f(void *a) {
    data d = *((data *) a);
    pthread_barrier_wait(d.barrier);
    int i;
    printf("Thread %d starting\n", d.id);
    for (i = 0; i < d.N; i++) {
        sem_wait(&d.sems[i]);
        printf("Thread %d has entered checkpoint %d\n", d.id, i);
        int n = (random() % 101 + 100);
        usleep(n * 1000);
        sem_post(&d.sems[i]);
    }
    printf("Thread %d finished\n", d.id);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide one argument\n");
        exit(1);
    }
    int i;
    int N = atoi(argv[1]);
    int M = 1;
    int step = 2;
    pthread_barrier_t barrier;
    for (i = 0; i < N; i++)
        M *= 2;
    sem_t *sems = malloc(sizeof(sem_t) * N);
    for (i = 0; i < N; i++) {
        sem_init(&sems[i], 0, M / step);
        step *= 2;
    }
    pthread_barrier_init(&barrier, NULL, M);
    pthread_t *T = malloc(sizeof(pthread_t) * M);
    data *args = malloc(sizeof(data) * M);
    for (i = 0; i < M; i++) {
        args[i].N = N;
        args[i].id = i;
        args[i].sems = sems;
        args[i].barrier = &barrier;
        pthread_create(&T[i], NULL, f, (void *) &args[i]);
    }
    for (i = 0; i < M; i++) {
        pthread_join(T[i], NULL);
    }
    pthread_barrier_destroy(&barrier);
    for (i = 0; i < N; i++) {
        sem_destroy(&sems[i]);
    }
    free(T);
    free(args);
    free(sems);
    return 0;
}

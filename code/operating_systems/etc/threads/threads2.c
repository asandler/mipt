#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define M 12

const int N = 1000000;
int counter = 0;
sem_t sem;

void* threadProcess(void *arg) {
    int j;
    for (j = 0; j < N; ++j) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }
    return (void *) 0;
}

int main() {
    int numbers[M];
    pthread_t threadID[M];
    int i;

    sem_init(&sem, 0, 1);
    for (i = 0; i < M; ++i) {
        numbers[i] = i;
        pthread_create(&(threadID[i]), 0, threadProcess, &(numbers[i]));
    }
    for (i = 0; i < M; ++i) {
        pthread_join(threadID[i], NULL);
    }
    printf("Counter = %d\n", counter);
    sem_destroy(&sem);
    return 0;
}

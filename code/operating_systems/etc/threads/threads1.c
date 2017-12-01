#include <stdio.h>
#include <pthread.h>

const int N = 120;
const int M = 6;
int sum[10];

void* threadProcess(void *arg) {
    int j, threadNumber;
    int cc = 0;
    threadNumber = *((int*)arg);
    for (j = threadNumber * (N / M); j < (threadNumber + 1) * (N / M); ++j) {
        sum[threadNumber] += j + 1;
    }
    return (void *) 0;
}

int main() {
    int numbers[M];
    int i;
    unsigned long long answer = 0;
    pthread_t threadID[M];
    for (i = 0; i < M; ++i) {
        sum[i] = 0;
    }
    for (i = 0; i < M; ++i) {
        numbers[i] = i;
        pthread_create(&(threadID[i]), 0, threadProcess, &(numbers[i]));
    }
    for (i = 0; i < M; ++i) {
        pthread_join(threadID[i], NULL);
    }
    for (i = 0; i < M; ++i) {
        answer += sum[i];
        printf("%d ", sum[i]);
    }
    printf("\nSum = %lld\n", answer);
    return 0;
}

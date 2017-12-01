#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define ASSERT(expr)           \
    if ((expr) < 0) {          \
        perror("shmem_get.c"); \
        exit(EXIT_FAILURE);    \
    }                          \

int main() {
    char* array;
    int shmid;
    char pathname[] = "shmem_put.c";
    key_t key;

    ASSERT(key = ftok(pathname, 0));

    ASSERT(shmid = shmget(key, 256 * sizeof(char), 0));

    if ((array = (char*)shmat(shmid, NULL, 0)) == (char*)(-1)) {
        perror("shmem_put.c");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", array);

    ASSERT(shmctl(shmid, IPC_RMID, 0));

    return 0;
}

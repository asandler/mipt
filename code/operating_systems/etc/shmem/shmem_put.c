#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ASSERT(expr)           \
    if ((expr) < 0) {          \
        perror("shmem_put.c"); \
        exit(EXIT_FAILURE);    \
    }                          \

int main() {
    char* array;
    int shmid;
    char pathname[] = "shmem_put.c";
    key_t key;

    ASSERT(key = ftok(pathname, 0));

    ASSERT(shmid = shmget(key, 256 * sizeof(char), 0666|IPC_CREAT|IPC_EXCL));

    if ((array = (char*)shmat(shmid, NULL, 0)) == (void *)(-1)) {
        perror("shmem_put.c");
        exit(EXIT_FAILURE);
    }

    int fd = open(pathname, O_RDONLY);
    int ind = 0;
    int res = 0;
    char s[2];

    while ((res = read(fd, s, 1)) > 0 && res != -1) {
        array[ind++] = s[0];
    }
    array[ind] = '\0';

    ASSERT(shmdt(array));

    return 0;
}

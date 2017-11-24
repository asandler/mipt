#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

struct string {
    char* data;
    int alloc_size, length;
};

struct string* init_string() {
    struct string* t = malloc(16);
    t->data = malloc(256);
    t->alloc_size = 256;
    t->length = 0;
    return t;
}

void safe_realloc(void* data, int size) {
    if ((data = realloc(data, size)) == NULL) {
        fprintf(stderr, "lsof: Out of memory");
        exit(1);
    }
}

void setNull(char* s) {
    int t;
    for (t = 0; s[t]; t++)
        s[t] = 0;
}

int isNumber(char* s) {
    int t;
    for (t = 0; s[t] != 0; t++)
        if (!isdigit(s[t]))
            return 0;
    return 1;
}

struct dirent* getNextNumberElement(DIR* dir) {
    struct dirent* data;
    while ((data = readdir(dir)) != NULL)
        if (isNumber(data->d_name))
            return data;
    return NULL;
}

void resize_string(struct string* s, int size) {
    if (s->alloc_size < size) {
        safe_realloc(s->data, size);
        s->alloc_size = size;
    }
}

int readLink(struct string* path, struct string* target) {
    if (readlink(path->data, target->data, target->alloc_size) == -1)
        return 0;
    while (readlink(path->data, target->data, target->alloc_size) == target->alloc_size)
        resize_string(target, target->alloc_size * 2);
    return 1;
}

void strCpy(struct string* target, char* source) {
    int slen = strlen(source);
    resize_string(target, slen * 2);
    strcpy(target->data, source);
    target->length = slen;
}

void strCat(struct string* target, char* source) {
    int slen = strlen(source);
    resize_string(target, (target->length + slen) * 2);
    strcat(target->data, source);
    target->length += slen;
}

unsigned int lsof() {
    DIR* proc_dir;
    DIR* fd_dir;
    struct dirent* proc_dir_data;
    struct dirent* fd_dir_data;
    struct string* link = init_string();
    struct string* command = init_string();
    struct string* proc_dir_name = init_string();
    struct string* fd_dir_name = init_string();
    struct string* path_to_opfd = init_string();
    struct string* exe_link = init_string();
    struct string* pid = init_string();

    proc_dir = opendir("/proc/");
    while ((proc_dir_data = getNextNumberElement(proc_dir)) != NULL) {
        strCpy(proc_dir_name, "/proc/");
        strCat(proc_dir_name, proc_dir_data->d_name);
        strCat(proc_dir_name, "/");
        strCpy(pid, proc_dir_data->d_name);
        strCpy(exe_link, proc_dir_name->data);
        strCat(exe_link, "exe");
        if (readLink(exe_link, command)) {
            strCpy(fd_dir_name, proc_dir_name->data);
            strCat(fd_dir_name, "fd/");
            fd_dir = opendir(fd_dir_name->data);
            while ((fd_dir_data = getNextNumberElement(fd_dir)) != NULL) {
                strCpy(path_to_opfd, fd_dir_name->data);
                strCat(path_to_opfd, fd_dir_data->d_name);
                if (readLink(path_to_opfd, link))
                    printf("%s\t%-24.24s\t%-24.24s\n", pid->data, command->data, link->data);
                setNull(link->data);
            }
            setNull(command->data);
            closedir(fd_dir);
        }
    }
    closedir(proc_dir);
    return 0;
}

int main() {
    printf("PID\tCOMMAND\t\t\t\tOPENED FILE\n");
    lsof();
    return 0;
}

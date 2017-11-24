#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "longstring.h"

#define CHECK(x) if (x != OK) return x;

int get_current_dir(string *s) {
        CHECK(resize_string(s, 1));
        while ((getcwd(s->data, s->alloc_size)) == NULL)
                CHECK(resize_string(s, s->alloc_size * 2));
        s->length = strlen(s->data);
        return OK;
}

int del_doubl_slash(string *s) {
        string temp;
        int i = 0;
        init_string(&temp);
        while (i < s->length) {
                if (s->data[i] != '/' || (s->data[i] == '/' && (i == 0 || temp.data[temp.length - 1] != '/')))
                        CHECK(add_to_string(&temp, s->data[i]));
                i++;
        }
        CHECK(copy_string(s, &temp));
        free_string(&temp);
        return OK;
}

int shift_lexem(string *s, string *res) {
        string temp, temp2;
        char c;
        init_string(&temp);
        init_string(&temp2);
        CHECK(copy_string(&temp2, s));
        CHECK(reverse_string(&temp2));
        if (temp2.data[temp2.length - 1] == '/') {
                CHECK(pop_string(&temp2, &c));
                CHECK(add_to_string(&temp, c));
        } else {
                while (temp2.length > 0 && temp2.data[temp2.length - 1] != '/') {
                        CHECK(pop_string(&temp2, &c));
                        CHECK(add_to_string(&temp, c));
                }
                if (temp2.length > 0) {
                        CHECK(pop_string(&temp2, &c));
                        CHECK(add_to_string(&temp, c));
                }
        }
        CHECK(reverse_string(&temp2));
        CHECK(copy_string(res, &temp));
        CHECK(copy_string(s, &temp2));
        free_string(&temp);
        free_string(&temp2);
        return OK;
}

int dot(string *s) {
        if (s->data[0] == '.') {
                if (s->length == 2)
                        return (s->data[1] == '/');
                if (s->length == 1)
                        return 1;
                return 0;
        }
        return 0;
}

int doubl_dot(string *s) {
        if (s->data[0] == '.' && s->data[1] == '.') {
                if (s->length == 3)
                        return (s->data[2] == '/');
                if (s->length == 2)
                        return 1;
                return 0;
        }
        return OK;
}

int jump_up(string *s) {
        char t;
        if (s->length > 1) {
                if (s->data[s->length - 1] == '/')
                        CHECK(pop_string(s, &t));
                while (s->data[s->length - 1] != '/')
                        CHECK(pop_string(s, &t));
        }
        return OK;
}

int merge(string *a, string *b, string *answer) {
        string temp, temp2, temp3;
        char c;
        init_string(&temp);
        init_string(&temp2);
        init_string(&temp3);
        CHECK(copy_string(&temp, a));
        CHECK(copy_string(&temp2, b));
        CHECK(add_to_string(&temp, '/'));
        CHECK(add_to_string(&temp2, '/'));
        del_doubl_slash(&temp);
        del_doubl_slash(&temp2);
        if (temp2.data[0] == '/') {
                CHECK(reverse_string(&temp2));
                CHECK(pop_string(&temp2, &c));
                CHECK(reverse_string(&temp2));
                temp.data[0] = '/';
                temp.length = 1;
        }
        while (temp2.length > 0) {
                shift_lexem(&temp2, &temp3);
                if (!dot(&temp3)) {
                        if (doubl_dot(&temp3))
                                jump_up(&temp);
                        else
                                CHECK(concat_string(&temp, &temp3));
                }
        }
        if (temp.length > 1)
                CHECK(pop_string(&temp, &c));
        CHECK(copy_string(answer, &temp));
        free_string(&temp);
        free_string(&temp2);
        free_string(&temp3);
        return OK;
}

int sym_link(string *s, string *answer) {
        string temp;
        char *data = malloc(s->length * sizeof(char) * 2);
        struct stat st;
        int t, i;
        init_string(&temp);
        bzero(data, s->length * sizeof(char) * 2);
        for (i = 0; i < s->length; i++)
                data[i] = s->data[i];
        if (stat(data, &st) != 0) {
                perror("realpath");
                return NOT_EXISTS;
        }
        if (data[strlen(data) - 1] == '/')
                data[strlen(data) - 1] = 0;
        CHECK(resize_string(&temp, 1));
        while (readlink(data, temp.data, temp.alloc_size) == temp.alloc_size)
                CHECK(resize_string(&temp, temp.alloc_size * 2));
        temp.length = strlen(temp.data);
        t = temp.length;
        if (t > 0)
                CHECK(copy_string(answer, &temp));
        free_string(&temp);
        free(data);
        return (t > 0) ? 1 : 0;
}

int kill_link(string *s) {
        int t, k;
        string temp, temp2, temp3, ans;
        init_string(&temp);
        init_string(&temp2);
        init_string(&temp3);
        init_string(&ans);
        CHECK(copy_string(&temp, s));
        do {
                CHECK(shift_lexem(&temp, &temp2));
                CHECK(concat_string(&ans, &temp2));
        } while ((k = sym_link(&ans, &temp3)) == 0 && temp.length > 0);
        if (k == NOT_EXISTS)
                return NOT_EXISTS;
        t = temp3.length;
        if (t > 0) {
                jump_up(&ans);
                CHECK(merge(&ans, &temp3, &ans));
                if (temp.length > 0)
                        CHECK(merge(&ans, &temp, &ans));
        }
        CHECK(copy_string(s, &ans));
        free_string(&temp);
        free_string(&temp2);
        free_string(&temp3);
        free_string(&ans);
        if (t > 0)
                return kill_link(s);
        return OK;
}

int main(int argc, char **argv) {
        string full, temp;
        int i, t, l;
        if (argc < 2) {
                fprintf(stderr, "realpath: Missing arguments\n");
                exit(EXIT_FAILURE);
        }
        for (t = 1; t < argc; t++) {
                init_string(&full);
                init_string(&temp);
                l = strlen(argv[t]);
                for (i = 0; i < l; i++)
                        if (add_to_string(&temp, argv[t][i]) != OK) {
                                fprintf(stderr, "realpath: out of memory\n");
                                return OUT_OF_MEMORY;
                        }
                if (get_current_dir(&full) == OK)
                        if (merge(&full, &temp, &full) == OK)
                                if (kill_link(&full) == OK)
                                        print_string(&full);
                free_string(&full);
                free_string(&temp);
        }
        return 0;
}

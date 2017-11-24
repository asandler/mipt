#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define OUT_OF_MEMORY 1
#define NULL_POINTER 2
#define NOT_EXISTS 3

typedef struct {
        char *data;
        size_t length, alloc_size;
} string;

/*
        This function initializes the string to work with it.
        No memory allocates here.
*/
int init_string(string *s) {
        if (s == NULL)
                return NULL_POINTER;
        s->data = NULL;
        s->alloc_size = s->length = 0;
        return OK;
}

/*
        This function resizes a string up to size*sizeof(char)
        bytes if its length was less than it. If size is equal
        to 0, resize_string is equal to free_string(), except
        that length of a string will not change.
*/
int resize_string(string *s, int size) {
        if (s == NULL)
                return NULL_POINTER;
        if ((s->data = realloc(s->data, size*(sizeof(char)))) == NULL)
                return OUT_OF_MEMORY;
        s->alloc_size = size;
        memset(&(s->data[s->length]), 0, s->alloc_size - s->length);
        return OK;
}

/*
        This function adds a character into a string
*/
int add_to_string(string *s, char a) {
        int error;
        if (s == NULL)
                return NULL_POINTER;
        if (s->alloc_size == 0)
                if ((error = resize_string(s, 1)) != OK)
                        return error;
        if (s->length == s->alloc_size)
                if ((error = resize_string(s, s->alloc_size * 2)) != OK)
                        return error;
        s->data[s->length++] = a;
        return OK;
}

/*
        This function sets the last char of given string to
        answer
*/
int pop_string(string *s, char *answer) {
        if (s == NULL)
                return NULL_POINTER;
        if (s->length == 0)
                return NULL_POINTER;
        *answer = s->data[s->length - 1];
        s->length--;
        return OK;
}

/*
        This function reverses the string
*/
int reverse_string(string *s) {
        int i;
        char t;
        if (s == NULL)
                return NULL_POINTER;
        for (i = 0; i < s->length / 2; i++) {
                t = s->data[i];
                s->data[i] = s->data[s->length - i - 1];
                s->data[s->length - i - 1] = t;
        }
        return OK;
}

/*
        This function reads string from the standart input until
        the EOF or '\n' "happens".
*/
int read_string(string *s) {
        int error;
        char c;
        if (s == NULL)
                return NULL_POINTER;
        while ((c = getchar()) != EOF && c != '\n')
                if ((error = add_to_string(s, c)) != OK)
                        return error;
        return OK;
}

/*
        This function prints string to the standart output
*/
int print_string(string *s) {
        int i;
        if (s == NULL)
                return NULL_POINTER;
        for (i = 0; i < s->length; i++)
                printf("%c", s->data[i]);
        printf("\n");
        fflush(stdout);
        return OK;
}

/*
        This function copies string source into string target.
*/
int copy_string(string *target, string *source) {
        int i, error;
        if (source == NULL || target == NULL)
                return NULL_POINTER;
        if (target != source)
                target->length = 0;
                if (source->data != NULL)
                        for (i = 0; i < source->length; i++)
                                if ((error = add_to_string(target, source->data[i])) != OK)
                                        return error;
        return OK;
}

/*
        This function concates two strings and puts answer into
        the first one.
*/
int concat_string(string *target, string *source) {
        int i, error;
        if (source == NULL || target == NULL)
                return NULL_POINTER;
        for (i = 0; i < source->length; i++)
                if ((error = add_to_string(target, source->data[i])) != OK)
                        return error;
        return OK;
}

/*
        This function frees all the allocated memory for the
        string.
*/
int free_string(string *s) {
        if (s == NULL)
                return NULL_POINTER;
        free(s->data);
        s->length = s->alloc_size = 0;
        return OK;
}

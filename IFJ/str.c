/**
 * Project - IFJ Projekt 2023
 *
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 *
 * @brief Implementation of custom variable-length string.
 */

#include "str.h"

void strInit(StringValue *s) {
    s->str = (char *) malloc(START_SIZE);
    if (s->str == NULL) {
        strFree(s);
        exit_program(99, "Memory allocation failure");
    }

    s->buffer = START_SIZE;
    strClear(s);
}

void strClear(StringValue *s) {
    s->length = 0;
    s->str[s->length] = '\0';
}

void strAppend(StringValue *s, int c) {
    if (s->length + 1 >= s->buffer) {
        s->str = (char *) realloc(s->str, s->length + ADD_SIZE);
        if (s->str == NULL) {
            strFree(s);
            exit_program(99, "Memory allocation failure");
        }
        s->buffer = s->length + ADD_SIZE;
    }
    s->str[s->length] = (char) c;
    s->length++;
    s->str[s->length] = '\0';
}

void strCopy(StringValue *source, StringValue *dest) {
    int newLength = source->length;
    if (newLength >= dest->buffer) {
        dest->str = (char *) realloc(dest->str, newLength + 1);
        if (dest->str == NULL) {
            strFree(dest);
            strFree(source);
            exit_program(99, "Memory allocation failure");
        }
        dest->buffer = newLength + 1;
    }

    strcpy(dest->str, source->str);
    dest->length = newLength;
}

void strCopyString(char *source, StringValue *dest) {
    int newLength = strlen(source);
    if (newLength >= dest->buffer) {
        dest->str = (char *) realloc(dest->str, newLength + 1);
        if (dest->str == NULL) {
            strFree(dest);
            exit_program(99, "Memory allocation failure");
        }
        dest->buffer = newLength + 1;
    }

    strcpy(dest->str, source);
    dest->length = newLength;
}

void strFree(StringValue *s) {
    free(s->str);
}

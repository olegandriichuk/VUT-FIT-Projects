/**
 * Project - IFJ Projekt 2023
 *
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 *
 * @brief Header file for str.c
 */

#ifndef STR_STR_H
#define STR_STR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"

#define START_SIZE 10
#define ADD_SIZE 5

/**
 * The main string structure
 */
typedef struct {
    char *str;
    int length;
    int buffer;
} StringValue;

/**
 *  Initialization of a custom string
 * @param str The string struct to initialize.
 */
void strInit(StringValue *str);

/**
 * Cleans the string
 * @param str The string struct to clean.
 */
void strClear(StringValue *str);

/**
 * Appends a character to the end of the string.
 * Reallocate memory if needed
 * @param str The string struct to modify.
 * @param c character to append
 */
void strAppend(StringValue *str, int c);

/**
 * Copy content from one string struct to another struct
 * @param source source string struct
 * @param dest destination string struct
 */
void strCopy(StringValue *source, StringValue *dest);

/**
 * Copy content from string to string struct
 * @param source source string
 * @param dest destination string struct
 */
void strCopyString(char *source, StringValue *dest);

/**
 *  Deinitialization of a custom string
 * @param str The string struct to deinitialize.
 */
void strFree(StringValue *str);

#endif //STR_STR_H
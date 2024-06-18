/**
 * Project - IFJ Projekt 2023
 *
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 *
 * @brief Header file for scanner.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "str.h"
#include "error.h"

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H

/**
 * States of fsm
 */
typedef enum {
    S_START,
    S_PLUS,
    S_MINUS,
    S_MULTIPLY,
    S_DIVIDE,
    S_COMMENT_LINE,
    S_COMMENT_LINES,
    S_COMMENT_LINES_END,
    S_LBRACKET,
    S_RBRACKET,
    S_CURL_LBRACKET,
    S_CURL_RBRACKET,
    S_COLON,
    S_ASSIGN,
    S_NOT_EQUAL,
    S_LESS,
    S_GREATER,
    S_NIL_OPERATOR,
    S_COMMA,
    S_EOF,
    S_STRING,
    S_STRING_END,
    S_STRING_ESC,
    S_STRING_PLUS,
    S_STRING_CHECK_0,
    S_STRING_CHECK_1,
    S_STRING_CHECK_2,
    S_STRING_CHECKOUT_0,
    S_STRING_CHECKOUT_1,
    S_STRING_CHECKOUT_2,
    S_HEX_OPEN,
    S_HEX,
    S_ID,
    S_INT,
    S_DOUBLE,
    S_DOUBLE_1,
    S_EXP_DOUBLE,
    S_EXP_DOUBLE_1,
    S_EXP_DOUBLE_2
} State;
/**
 * Keywords types
 */
typedef enum {
    K_LET,
    K_VAR,
    K_FUNC,
    K_RETURN,
    K_IF,
    K_ELSE,
    K_WHILE,
    K_NIL,
    K_INT,
    K_DOUBLE,
    K_STRING
} Keywords;

/**
 * Token types
 */
typedef enum Types {
    L_EOF,
    L_KEYWORD, // func
    L_KEYWORD_QMARK, // Int?
    L_ID, // id
    L_ID_EXMARK, // id?
    L_LBRACKET,// (
    L_RBRACKET, // )
    L_CURL_LBRACKET, // {
    L_CURL_RBRACKET, // }
    L_COLON, // :
    L_ASSIGN, // =
    L_EQUAL, // ==
    L_NOT_EQUAL, // !=
    L_PLUS, // +
    L_MINUS, // -
    L_ARROW, // ->
    L_MULTIPLY, // *
    L_DIVIDE, // /
    L_LESS, // <
    L_LESS_EQUAL, // <=
    L_GREATER, // >
    L_GREATER_EQUAL, // >=
    L_NIL_OPERATOR, // ??
    L_COMMA, // ,
    L_INT, // 2023
    L_DOUBLE, // 2023.12
    L_EXP, // 2.1e4
    L_STRING, // "Happy New Year"
    L_STRING_PLUS, // """Happy New Year"""
    L_EMPTY //
} Types;

/**
 * Token data
 */
typedef struct tokeData{
    StringValue stringValue;
    int integer;
    double double_num;
    Keywords keyword;
} TokenData;

/**
 * The main token structure
 */
typedef struct token {
    Types type;
    TokenData data;
} Token;

/**
 * FSM
 * Allocates memory for the token
 * reads information according to the current state,
 * the next state and the current character
 * @param file source
 * @return built token
 */
Token *getToken(FILE *file);

/**
 * Checks if the given string is a keyword
 * @param token active token
 * @param string id name
 */
void checkKeyword(Token *token, StringValue *string);
/**
 * Converts a hex number to a decimal number
 * Then append symbol to active string
 * @param hex hex number
 * @param string
 */
void convertHex(StringValue *hex, StringValue *string);

#endif //SCANNER_SCANNER_H
/**
 * Project - IFJ Projekt 2023
 *
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 *
 * @brief Scanner for parsing and returning tokens
 */

#include "scanner.h"

/**
 * Safe exit from the program in case of error
 * @param code error code
 * @param string active string
 * @param message error message
 */
static void exitFree(int code, StringValue *string, char *message) {
    strFree(string);
    exit_program(code, message);
}

void convertHex(StringValue *hex, StringValue *string) {
    if (hex->length >= 9) {
        strFree(hex);
        exitFree(1, string,"String contains unsupported character");
    }
    char *endPtr = NULL;
    int decimal = strtol(hex->str, &endPtr, 16);
    if (*endPtr != '\0') {
        exitFree(0, 0, "Hexadecimal conversion has failed");
    }
    strAppend(string, decimal);
}

void checkKeyword(Token *token, StringValue *string) {
    if (strcmp(string->str, "let") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_LET;
    } else if (strcmp(string->str, "var") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_VAR;
    } else if (strcmp(string->str, "func") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_FUNC;
    } else if (strcmp(string->str, "return") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_RETURN;
    } else if (strcmp(string->str, "if") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_IF;
    } else if (strcmp(string->str, "else") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_ELSE;
    } else if (strcmp(string->str, "while") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_WHILE;
    } else if (strcmp(string->str, "Int") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_INT;
    } else if (strcmp(string->str, "Double") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_DOUBLE;
    } else if (strcmp(string->str, "String") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_STRING;
    } else if (strcmp(string->str, "nil") == 0) {
        token->type = L_KEYWORD;
        token->data.keyword = K_NIL;
    } else {
        token->type = L_ID;
    }

    strInit(&token->data.stringValue);
    strCopy(string, &token->data.stringValue);
}

Token *getToken(FILE *file) {
    Token *token = (Token *) malloc(sizeof(Token));
    if (token == NULL) {
        exit_program(99, "Memory allocation failure");
    }
    StringValue string;
    StringValue hex;
    strInit(&string);

    bool tokenBuilt = false;
    State state = S_START;
    token->type = L_EMPTY;

    int c;
    while (!tokenBuilt) {
        c = getc(file);

        switch (state) {
            case S_START:
                switch (c) {
                    case ' ':
                        state = S_START;
                        break;
                    case '+':
                        state = S_PLUS;
                        break;
                    case '-':
                        state = S_MINUS;
                        break;
                    case '*':
                        state = S_MULTIPLY;
                        break;
                    case '/':
                        state = S_DIVIDE;
                        break;
                    case '(':
                        state = S_LBRACKET;
                        break;
                    case ')':
                        state = S_RBRACKET;
                        break;
                    case '{':
                        state = S_CURL_LBRACKET;
                        break;
                    case '}':
                        state = S_CURL_RBRACKET;
                        break;
                    case ':':
                        state = S_COLON;
                        break;
                    case '=':
                        state = S_ASSIGN;
                        break;
                    case '!':
                        state = S_NOT_EQUAL;
                        break;
                    case '<':
                        state = S_LESS;
                        break;
                    case '>':
                        state = S_GREATER;
                        break;
                    case '?':
                        state = S_NIL_OPERATOR;
                        break;
                    case ',':
                        state = S_COMMA;
                        break;
                    case EOF:
                        state = S_EOF;
                        break;
                    case '\"':
                        state = S_STRING_CHECK_0;
                        break;
                    default:
                        if (isalpha(c) || c == '_') {
                            state = S_ID;
                            ungetc(c, file);
                        } else if (isdigit(c)) {
                            state = S_INT;
                            ungetc(c, file);
                        } else if (isspace(c)) {
                            // nothing
                        } else {
                            exitFree(1, &string, "Unknown token found");
                        }
                        break;

                }
                break;
            case S_PLUS:
                token->type = L_PLUS;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_MINUS:
                if (c == '>')
                    token->type = L_ARROW;
                else {
                    token->type = L_MINUS;
                    ungetc(c, file);
                }
                tokenBuilt = true;
                break;
            case S_MULTIPLY:
                token->type = L_MULTIPLY;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_LBRACKET:
                token->type = L_LBRACKET;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_RBRACKET:
                token->type = L_RBRACKET;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_CURL_LBRACKET:
                token->type = L_CURL_LBRACKET;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_CURL_RBRACKET:
                token->type = L_CURL_RBRACKET;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_COLON:
                token->type = L_COLON;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_COMMA:
                token->type = L_COMMA;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_EOF:
                token->type = L_EOF;
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_LESS:
                if (c == '=')
                    token->type = L_LESS_EQUAL;
                else {
                    token->type = L_LESS;
                    ungetc(c, file);
                }
                tokenBuilt = true;
                break;
            case S_GREATER:
                if (c == '=')
                    token->type = L_GREATER_EQUAL;
                else {
                    token->type = L_GREATER;
                    ungetc(c, file);
                }
                tokenBuilt = true;
                break;
            case S_ASSIGN:
                if (c == '=')
                    token->type = L_EQUAL;
                else {
                    token->type = L_ASSIGN;
                    ungetc(c, file);
                }
                tokenBuilt = true;
                break;
            case S_NOT_EQUAL:
                if (c == '=') {
                    token->type = L_NOT_EQUAL;
                    tokenBuilt = true;
                } else {
                    exitFree(1, &string, "Unknown token found");
                }
                break;
            case S_NIL_OPERATOR:
                if (c == '?') {
                    token->type = L_NIL_OPERATOR;
                    tokenBuilt = true;
                } else {
                    exitFree(1, &string, "Unknown token found");
                }
                break;
            case S_ID:
                if (isalpha(c) || isdigit(c) || c == '_') {
                    strAppend(&string, c);
                } else {
                    checkKeyword(token, &string);

                    if (c == '?' && token->type == L_KEYWORD &&
                        (token->data.keyword == K_INT || token->data.keyword == K_DOUBLE ||
                         token->data.keyword == K_STRING)) {
                        token->type = L_KEYWORD_QMARK;
                    } else if (c == '!' && token->type == L_ID) {
                        token->type = L_ID_EXMARK;
                    } else {
                        ungetc(c, file);
                    }
                    tokenBuilt = true;
                }
                break;
            case S_INT:
                if (isdigit(c)) {
                    strAppend(&string, c);
                } else if (c == '.') {
                    strAppend(&string, c);
                    state = S_DOUBLE;
                } else if (c == 'e' || c == 'E') {
                    strAppend(&string, c);
                    state = S_EXP_DOUBLE;
                } else {
                    ungetc(c, file);
                    char *endPtr = NULL;
                    int value = (int) strtoul(string.str, &endPtr, 10);
                    if (*endPtr != '\0') {
                        exitFree(1, &string, "Integer conversion has failed");
                    }
                    token->data.integer = value;
                    token->type = L_INT;
                    tokenBuilt = true;
                }
                break;
            case S_DOUBLE:
                if (isdigit(c)) {
                    state = S_DOUBLE_1;
                    ungetc(c, file);
                } else {
                    exitFree(1, &string, "Unknown token found, dot must be followed by a digit");
                }
                break;
            case S_DOUBLE_1:
                if (isdigit(c)) {
                    strAppend(&string, c);
                } else if (c == 'e' || c == 'E') {
                    state = S_EXP_DOUBLE;
                    strAppend(&string, c);
                } else {
                    ungetc(c, file);
                    char *endPtr = NULL;
                    double value = strtod(string.str, &endPtr);
                    if (*endPtr != '\0'){
                        exitFree(1, &string, "Float conversion has failed");
                    }
                    token->data.double_num = value;
                    token->type = L_DOUBLE;
                    tokenBuilt = true;
                }
                break;
            case S_EXP_DOUBLE:
                if (c == '+' || c == '-') {
                    strAppend(&string, c);
                    state = S_EXP_DOUBLE_1;
                } else if (isdigit(c)) {
                    state = S_EXP_DOUBLE_1;
                    ungetc(c, file);
                } else {
                    exitFree(1, &string, "Unknown token found, exponent must be followed by a (signed) digit");
                }
                break;
            case S_EXP_DOUBLE_1:
                if (isdigit(c)) {
                    strAppend(&string, c);
                    state = S_EXP_DOUBLE_2;
                } else {
                    exitFree(1, &string, "Unknown token found, exponent must be followed by a (signed) digit");
                }
                break;
            case S_EXP_DOUBLE_2:
                if (isdigit(c)) {
                    strAppend(&string, c);
                } else {
                    ungetc(c, file);
                    char *endPtr = NULL;
                    double value = strtod(string.str, &endPtr);
                    if (*endPtr != '\0') {
                        exitFree(1, &string, "Float conversion has failed");
                    }
                    token->data.double_num = value;
                    token->type = L_EXP;
                    tokenBuilt = true;
                }
                break;
            case S_STRING_CHECK_0:
                if (c == '\"') {
                    state = S_STRING_CHECK_1;
                } else {
                    ungetc(c, file);
                    state = S_STRING;
                }
                break;
            case S_STRING_CHECK_1:
                if (c == '\"') {
                    state = S_STRING_CHECK_2;
                } else {
                    token->type = L_STRING;
                    strInit(&token->data.stringValue);
                    strCopy(&string, &token->data.stringValue);
                    tokenBuilt = true;
                    ungetc(c, file);
                }
                break;
            case S_STRING_CHECK_2:
                if (c == '\n') {
                    token->type = L_STRING_PLUS;
                    state = S_STRING_PLUS;
                } else {
                    exitFree(1, &string,
                             "Unknown token found, multi-line string starts and ends with three double quotes on a separate line");
                }
                break;
            case S_STRING:
                if (c < 32) {
                    exitFree(1, &string,"String contains unsupported character");
                } else if (c == '\"') {
                    state = S_STRING_END;
                } else if (c == '\\') {
                    state = S_STRING_ESC;
                } else {
                    strAppend(&string, c);
                }
                break;
            case S_STRING_PLUS:
                if (c == '\n') {
                    state = S_STRING_CHECKOUT_0;
                } else if (c == '\\') {
                    state = S_STRING_ESC;
                } else if (c < 32) {
                    exitFree(1, &string,"String contains unsupported character");
                } else {
                    strAppend(&string, c);
                }
                break;
            case S_STRING_END:
                token->type = L_STRING;
                strInit(&token->data.stringValue);
                strCopy(&string, &token->data.stringValue);
                tokenBuilt = true;
                ungetc(c, file);
                break;
            case S_STRING_CHECKOUT_0:
                if (c == '\"') {
                    state = S_STRING_CHECKOUT_1;
                } else {
                    strAppend(&string, '\n');
                    strAppend(&string, c);
                    state = S_STRING_PLUS;
                }
                break;
            case S_STRING_CHECKOUT_1:
                if (c == '\"') {
                    state = S_STRING_CHECKOUT_2;
                } else {
                    strAppend(&string, '\"');
                    strAppend(&string, c);
                    state = S_STRING_PLUS;
                }
                break;
            case S_STRING_CHECKOUT_2:
                if (c == '\"') {
                    strInit(&token->data.stringValue);
                    strCopy(&string, &token->data.stringValue);
                    tokenBuilt = true;
                } else {
                    strAppend(&string, '\"');
                    strAppend(&string, '\"');
                    strAppend(&string, c);
                    state = S_STRING_PLUS;
                }
                break;
            case S_STRING_ESC:
                if (token->type == L_STRING_PLUS) {
                    state = S_STRING_PLUS;
                } else {
                    state = S_STRING;
                }
                switch (c) {
                    case '\\':
                        strAppend(&string, '\\');
                        break;
                    case '\"':
                        strAppend(&string, '\"');
                        break;
                    case 'n':
                        strAppend(&string, '\n');
                        break;
                    case 't':
                        strAppend(&string, '\t');
                        break;
                    case 'r':
                        strAppend(&string, '\r');
                        break;
                    case 'u':
                        state = S_HEX_OPEN;
                        break;
                    default:
                        exitFree(1, &string,"Unknown escape character");
                        break;
                }
                break;
            case S_HEX_OPEN:
                if (c == '{') {
                    strInit(&hex);
                    state = S_HEX;
                } else {
                    exitFree(1, &string,"String contains unsupported character");
                }
                break;
            case S_HEX:
                if (isxdigit(c)) {
                    strAppend(&hex, c);
                } else if (c == '}') {
                    convertHex(&hex, &string);
                    strFree(&hex);
                    if (token->type == L_STRING_PLUS) {
                        state = S_STRING_PLUS;
                    } else {
                        state = S_STRING;
                    }
                } else {
                    exitFree(1, &string,"String contains unsupported character");
                }
                break;
            case S_DIVIDE:
                if (c == '/')
                    state = S_COMMENT_LINE;
                else if (c == '*') {
                    state = S_COMMENT_LINES;
                } else {
                    token->type = L_DIVIDE;
                    tokenBuilt = true;
                    ungetc(c, file);
                }
                break;
            case S_COMMENT_LINE:
                if (c == '\n' || c == EOF)
                    state = S_START;
                break;
            case S_COMMENT_LINES:
                if (c == EOF) {
                    exitFree(1, &string,"Unterminated multi-line comment");
                } else if (c == '*')
                    state = S_COMMENT_LINES_END;
                break;
            case S_COMMENT_LINES_END:
                if (c == '/') {
                    state = S_START;
                } else if (c == EOF) {
                    exitFree(1, &string,"Unterminated multi-line comment");
                } else
                    state = S_COMMENT_LINES;
                break;
        }
    }

    strFree(&string);
  
    return token;
}
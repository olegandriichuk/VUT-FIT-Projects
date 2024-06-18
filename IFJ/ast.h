#ifndef IFJ23_AST_H
#define IFJ23_AST_H

#include "scanner.h"

typedef struct ast_node {
    Token *token;          // token struct
    struct ast_node *left;   // left child
    struct ast_node *right;  // right child
} ast_node_t;

ast_node_t *make_leaf (Token *token);

ast_node_t* make_tree(Token *token, ast_node_t* left, ast_node_t* right);

int ast_tree_height(ast_node_t* tree);

int max_number(int number1, int number2);

#endif //IFJ23_AST_H
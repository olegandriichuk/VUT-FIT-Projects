#include "ast.h"

ast_node_t *make_leaf(Token *token) {
    ast_node_t *new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
    }
    new->token = token;
    new->left = NULL;
    new->right = NULL;

    return new;
}

ast_node_t *make_tree(Token *token, ast_node_t *left, ast_node_t *right) {
    ast_node_t *new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "Memory allocation failure");
    }
    new->token = token;
    new->left = left;
    new->right = right;

    return new;
}

int max_number(int number1, int number2) {
    return (number1 > number2) ? number1 : number2;
}

int ast_tree_height(ast_node_t *tree) {
    if (tree != NULL)
        return max_number(ast_tree_height(tree->left), ast_tree_height(tree->right)) + 1;
    else
        return 0;
}
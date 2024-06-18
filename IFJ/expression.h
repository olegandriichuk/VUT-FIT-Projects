/**
 * Project - IFJ Projekt 2023
 *
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 *
 * @brief Header file for expression.c
 */

#ifndef IFJ23_EXPRESSION_H
#define IFJ23_EXPRESSION_H

#include "scanner.h"
#include "ast.h"
#include "parser.h"
#include "symtable.h"
#include "stack_for_trees.h"

/**
 * Precedence table actions
 */
typedef enum prec_symbols {
    PLUS, // +
    MUL, // *
    LBRACKET, // (
    RBRACKET, // )
    ID, // i
    DOLLAR, // $
    MINUS, // -
    DIV, // /
    LESS, // <
    GREATER, // >
    LESS_EQUAL,  // <=
    GREATER_EQUAL, // >=
    EQUAL, // ==
    NOT_EQUAL, // !=
    EMPTY,
    W, // = wait
    R, // > reduce
    S, // < shift
    N, // error
    E, // E
    INVALID
} prec_symbols;

/**
 * The stack element structure
 */
typedef struct stack_el {
    prec_symbols data;
    Token *token;
    ast_node_t* tree;
    struct stack_el *next;
} Stack_item;

/**
 * The stack structure
 */
typedef struct {
    Stack_item *top;
} Stack_exp;

/**
 * Stack initialization
 * @param stack The stack struct to init
 */
void stackInit(Stack_exp *stack);

/**
 * inserts new value to stack, new value becomes stack top
 * @param stack pointer to stack
 * @param data enum value to be pushed on stack
 */

/**
* Inserts new element to stack
* @param stack The stack struct to modify
* @param data element to add
*/
void stackPush(Stack_exp *stack, prec_symbols data);

/**
 * Inserts specific element shift in precedence table under current token
 * @param stack The stack struct to modify
 */
void stackShiftPush(Stack_exp *stack);

/**
 * Removes current top element from the stack
 * @param stack The stack struct to modify
 * @return popped element
 */
Stack_item *stackPop(Stack_exp* stack);

/**
 * Finds top token on stack, ignor terminals
 * @param stack The stack struct to modify
 * @return prec symbol
 */
prec_symbols stack_top_terminal(Stack_exp *stack);

/**
 * Ð¡heck if variable is defined,
 * Check the final type of the expression
 * @param current_token active token
 * @param new_node pointer to the variable data
 * @param main_stack pointer to the program data stack
 * @param exp_data_type data type of expression
 * @return error_code: success = 0, other = error
 */
int id_define_check(Token *current_token, node_data *new_node, Stack *main_stack, id_data_type *exp_data_type);

/**
 * Based on the last two tokens
 * will check if it's the end of expression
 * @param current_token active token
 * @param prev_token previous token
 * @param new_node pointer to the variable data
 * @param main_stack pointer to the program data stack
 * @param current_token_enum prec data of active token
 * @param exp_data_type data type of expression
 * @param is_expression is expression or condition
 * @return error_code: success = 0, other = error
 */
int exp_end_check(Token *current_token, Token *prev_token, node_data *new_node, Stack *main_stack,
                  prec_symbols *current_token_enum, id_data_type *exp_data_type, bool is_expression);

/**
 * The main function, where precedence rules must be found.
 * Converts tokens into ast
 * Checks the syntax and semantics of the expression
 * @param used_token pointer to incoming token from parser
 * @param is_expression is expression or condition
 * @param new_node pointer to the variable data
 * @param main_stack pointer to the program data stack
 * @param tree expression result ast
 * @return error_code: success = 0, other = error
 */
int expression(Token * used_token, bool is_expression, node_data* new_node, Stack* main_stack, ast_node_t** tree);

/**
 * Checks data in stack for right final positions
 * @param stack The stack struct
 * @return result 1 is ok, else 0
 */
int builtCorrect(Stack_exp *stack);

/**
 * Reduce number of nonterminals and terminals on stack
 * Calls AST generation if needed
 * @param stack The stack struct to modify
 * @return result 1 is ok, else 0
 */
int rule_reduction(Stack_exp *stack);

/**
 * Checks prec data representing by token
 * @param token pointer to current token
 * @return specific prec symbol
 */
prec_symbols check_token_symbol(Token *token);

#endif //IFJ23_EXPRESSION_H
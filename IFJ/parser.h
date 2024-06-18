#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "stack_for_trees.h"
#include "symtable.h"
#include "ast.h"
#include "expression.h"

//Main function for start of syntax analysis
void parse();

//Beginning of the LL-grammar, if file is not empty, procceed to analysis
void program();

//Function for reading parameters while function declaration
void params();

//Function for checking if type of token is as expected
void type_check(Types type, char *err_msg, bool newTok);

//Function-helper for params, to support more parameters
void params_n();

//Function for choosing next behaviour, according to LL-grammar
void statement();

//Function for calling declared functions and starting to analyze arguments
void fnc_call(char *fnc_name);

//Function for reading arguments while function calling process
void args(char *curr);

//Function-helper for args
void args_n(func_params *param, int param_num);

//Function for first token traversal to find all declared functions
void parse_for_func();

//Function to create stractures for future use
void create_new_node_and_str(node_data *new_node, StringValue *str, bool nil, int param, id_type s_type,
                             id_data_type d_type);

//Function to see if id was declared
void check_if_token_declared();

void counter_dec();

void exp_check(Token *token, bool is_expression, node_data *new_node);

//Function for first parameters initialization
void starting_routine();

//Function for adding builtin functions to global scope
void add_builtin_func();

//Function for initialization of the array of tokens
void array_init();

//Function for initial storing all tokens to main_array
void store_to_array();

//Cycle for calling store_to_array()
void save_tokens();

//Function for working of expression analysis
Token *exp_token();

//Function to get new token from array
void newToken();

//Function for checking invalid statement
void statement_check(Token token);

extern FILE *file;
#endif
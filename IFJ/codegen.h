#ifndef IFJ_CODEGEN_H
#define IFJ_CODEGEN_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "error.h"
#include "symtable.h"
#include "str.h"

/**
 * function which formates normal string to IFJcode23 string
 * @param str string to formate
 * @return return formatted string
 */
char* create_string_4_gen( const char* str);
/**
 * generate head program
 */
void gen_head();
/**
 * function which ends main function
 */
void gen_end_of_main();
/**brief
 * generates error cases
 */
void gen_exit_from_label();
/**
 * generates builtin function readString
 */
void gen_readString();
/**
 * generates builtin function readInt
 */
void gen_readInt();
/**
 * generates builtin function readDouble
 */
void gen_readDouble();

/**
 * generates builtin function write
 */
void gen_write();

/**
 * generates builtin function Int2Double
 */
void gen_Int2Double();
/**
 * generates builtin function Double2Int
 */
void gen_Double2Int();

/**
 * generates builtin function StringLen
 */
void gen_StringLen();
/**
 * generates builtin function substring
 */
void gen_substring();
/**
 * generates builtin function ord
 */
void gen_ord();
/**
 * generates builtin function chr
 */
void gen_chr();
/**
 *
 * @param id_func identifier of function
 * @param label_num number of special label
 */
void generate_func_header(char* id_func, int label_num);
/**brief
 *
 * @param label_num  number of special label
 * @param data_of_return data of function return
 */
void gen_func_return(int label_num, bst_node_t* data_of_return);
/**
 * go through the bst recursively , find and declare variables
 * @param root pointer to the to of bst
 * @param is_local local or global variable
 */
void inorderTraversal_bst(bst_node_t* root, bool is_local);
/**
 *
 * @param root  pointer to the to of bst
 * @param func_id identifier of  function
 * @param is_local local or global variable
 */
void gen_func_dec(bst_node_t* root, char* func_id, bool is_local);
/**
 *
 * @param var_id identifier of variable
 * @param local local or global variable
 */
void generate_var_definition(char* var_id, bool local);
/**
 * calls functions
 * @param func_id identifier of function
 */
void gen_func_call(char* func_id);

/**
 * exit from the function
 */
void gen_exit_from_func();

/**
 * checks if operands have right types
 *
 */
void gen_check_expr_types();
/**
 * generates expression with one operand
 * @param token
 * @param is_local local or global expression
 */
void gen_one_operand_expr(Token* token, bool is_local);
/**brief
 *
 * @param is_in_func local or global expression
 * @param label_num number of special label
 * @param current_node pointer to AST root node
 */
void gen_ast(bool is_in_func, int* label_num, ast_node_t* current_node);
#endif //IFJ_CODEGEN_H

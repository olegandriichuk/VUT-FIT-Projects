#include "codegen.h"

char* create_string_4_gen( const char* str){
    StringValue tmp;
    strInit(&tmp);
    char esc_sequence[5] = "";
    char c;
    for (int i = 0; (c = str[i]) != '\0'; ++i) {
        if(c < 33 || c == 35 || c == 92 || c > 126){
            strAppend(&tmp, '\\');
            sprintf(esc_sequence, "%03d", c);
            for (int j = 0; j < 3; ++j) {
                strAppend(&tmp, esc_sequence[j]);
            }
        }else{
            strAppend(&tmp, c);
        }
    }
    return tmp.str;
}

void gen_head(){
    printf(".IFJcode23\n");
    printf("DEFVAR GF@tmp_var\n");

    printf("DEFVAR GF@ast_expr1\n");
    printf("DEFVAR GF@expr1_type\n");

    printf("DEFVAR GF@ast_expr2\n");
    printf("DEFVAR GF@expr2_type\n");

    printf("JUMP __main__\n");
    gen_exit_from_label();
    gen_readString();
    gen_readInt();
    gen_readDouble();
    gen_chr();
    gen_ord();
    gen_write();
    gen_StringLen();
    gen_substring();
    gen_Double2Int();
    gen_Int2Double();
    printf("LABEL __main__\n");
    //printf("CREATEFRAME\n");
    //printf("PUSHFRAME\n");
    printf("JUMP __main_declaration__\n");
    printf("LABEL __do_main__\n");
}
void gen_end_of_main() {
    printf("LABEL __main_return__\n");
    printf("EXIT int@0\n");
}
void gen_exit_from_label(){
    // wrong type of parameter in func
    printf("LABEL __error_sem_type_check__\n");
    printf("EXIT int@4\n");
    // undefined variable
    printf("LABEL __undefined_var__\n");
    printf("EXIT int@5\n");
    // wrong expression of func-return
    printf("LABEL __wrong_return_exp__\n");
    printf("EXIT int@6\n");
    // wrong operand types
    printf("LABEL __wrong_operand_types__\n");
    printf("EXIT int@7\n");
    // not defined or not known type of var or param
    printf("LABEL __not_known_type__\n");
    printf("EXIT int@8\n");

    printf("LABEL __other_sem_errors__\n");
    printf("EXIT int@9\n");
}
void gen_readString(){
    printf("LABEL __readString__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@readString\n");
    printf("READ LF@readString string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}
void gen_readInt(){
    printf("LABEL __readInt__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@readInt\n");
    printf("READ LF@readInt int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_readDouble(){
    printf("LABEL __readDouble__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@readDouble\n");
    printf("READ LF@readDouble float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_chr(){

    printf("LABEL __chr__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@ret_value\n");
    printf("MOVE LF@ret_value string@\n");
    printf("DEFVAR LF@condition\n");
    printf("LT LF@condition LF@number int@0\n");
    printf("JUMPIFEQ __char_return__ LF@condition bool@true\n");
    printf("GT LF@condition LF@number int@255\n");
    printf("JUMPIFEQ __char_return__ LF@condition bool@true\n");
    printf("INT2CHAR LF@ret_value LF@number\n");
    printf("LABEL __char_return__\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_ord(){

    printf("LABEL __ord__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@ret_value\n");
    printf("MOVE LF@ret_value int@0\n");
    printf("DEFVAR LF@param_c\n");
    printf("MOVE LF@param_c  LF@string_val\n");
    printf("DEFVAR LF@type_of_var\n");
    printf("TYPE LF@type_of_var LF@param_c\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type_of_var string@string\n");
    printf("JUMPIFEQ __receive_nothing__ LF@param_c string@\n");
    printf("STRI2INT LF@ret_value LF@param_c int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL __receive_nothing__\n");
    printf("MOVE LF@ret_value int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_write(){
    printf("LABEL __write__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@current_term\n");
    printf("POPS LF@i\n");
    printf("LABEL __write_loop__\n");
    printf("JUMPIFEQ __write_loop_end__ int@0 LF@i\n");
    printf("SUB LF@i LF@i int@1\n");
    printf("POPS LF@current_term\n");
    printf("WRITE LF@current_term\n");
    printf("JUMP __write_loop__\n");
    printf("LABEL __write_loop_end__\n");
    printf("PUSHS nil@nil\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_StringLen(){
    printf("LABEL __StringLen__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@str\n");
    printf("DEFVAR LF@type_of_param\n");
    printf("TYPE LF@type_of_param LF@param1\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type_of_param string@string\n");
    printf("DEFVAR LF@ret_value\n");
    printf("DEFVAR LF@type_of_param\n");
    printf("TYPE LF@type_of_param LF@param1\n");
    printf("JUMPIFEQ __string_is_null__ LF@type_of_param string@nil\n");
    printf("STRLEN LF@ret_value LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL __string_is_null__\n");
    printf("MOVE LF@ret_value int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_substring(){
    printf("LABEL __substring__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@String\n");
    printf("DEFVAR LF@begin\n");
    printf("DEFVAR LF@end\n");
    printf("MOVE LF@String LF@str\n");
    printf("MOVE LF@begin LF@i\n");
    printf("MOVE LF@end LF@j\n");
    printf("DEFVAR LF@result_str\n");
    printf("MOVE LF@result_str string@\n");
    printf("JUMPIFEQ __string_emp__ LF@String nil@nil\n");

    printf("DEFVAR LF@type\n");
    printf("TYPE LF@type LF@String\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type string@string\n");
    printf("TYPE LF@type LF@begin\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type string@int\n");
    printf("TYPE LF@type LF@end\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type string@int\n");

    printf("DEFVAR LF@bool_tmp\n");


    printf("LT LF@bool_tmp LF@begin int@0\n");
    printf("JUMPIFEQ __substr_is_nil__ LF@bool_tmp bool@true\n");

    printf("LT LF@bool_tmp LF@end int@0\n");
    printf("JUMPIFEQ __substr_is_nil__ LF@bool_tmp bool@true\n");

    printf("GT LF@bool_tmp LF@begin LF@end\n");
    printf("JUMPIFEQ __substr_is_nil__ LF@bool_tmp bool@true\n");

    printf("DEFVAR LF@length_str\n");
    printf("STRLEN LF@length_str LF@String\n");
    printf("GT LF@bool_tmp LF@begin LF@length_str\n");
    printf("JUMPIFEQ __substr_is_nil__ LF@bool_tmp bool@true\n");
    printf("EQ LF@bool_tmp LF@begin LF@length_str\n");
    printf("JUMPIFEQ __substr_is_nil__ LF@bool_tmp bool@true\n");


    printf("GT LF@bool_tmp LF@end LF@length_str\n");
    printf("JUMPIFEQ __substr_is_nil__ LF@bool_tmp bool@true\n");
    printf("JUMPIFEQ __string_emp__ LF@begin LF@end\n");

    printf("DEFVAR LF@var1\n");
    printf("DEFVAR LF@var2\n");
    printf("LABEL substring_not_null\n");
    printf("GETCHAR LF@var1 LF@String LF@begin\n");
    printf("CONCAT LF@result_str LF@result_str LF@var1\n");
    printf("ADD LF@begin  LF@begin int@1\n");
    printf("LT LF@var2 LF@begin LF@end\n");
    printf("JUMPIFEQ substring_not_null LF@var2 bool@true\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL __string_emp__\n");
    printf("MOVE LF@result_str string@\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL __substr_is_nil__\n");
    printf("MOVE LF@result_str nil@nil\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_Int2Double(){
    printf("LABEL __Int2Double__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@int_var\n");
    printf("MOVE LF@int_var LF@number\n");
    printf("DEFVAR LF@result\n");
    printf("DEFVAR LF@type\n");
    printf("TYPE LF@type LF@int_var\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type string@int\n");
    printf("INT2FLOAT LF@result LF@int_var\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_Double2Int(){
    printf("LABEL __Double2Int__\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@double_var\n");
    printf("MOVE LF@double_var LF@number\n");
    printf("DEFVAR LF@result\n");
    printf("DEFVAR LF@type\n");
    printf("TYPE LF@type LF@double_var\n");
    printf("JUMPIFNEQ __error_sem_type_check__ LF@type string@float\n");
    printf("FLOAT2INT LF@result LF@double_var\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}
void generate_func_header(char* id_func,int label_num) {
    printf("JUMP __main_%d__\n", label_num);
    printf("LABEL __%s__\n", id_func);
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@ret_value\n");
    printf("MOVE LF@ret_value nil@nil\n");
    printf("LABEL __do_%s__\n", id_func);
    printf("JUMP __%s_declaration__\n", id_func);
}
void gen_func_return(int label_num, bst_node_t* data_of_return) {
    printf("LABEL __%s_return__\n", data_of_return->key);
    if (data_of_return->node_data->data_type == DTYPE_INT) {
        printf("TYPE GF@expr1_type LF@ret_value\n");
        if (data_of_return->node_data->can_be_nil != 0) {
            printf("JUMPIFNEQ __error_sem_type_check__  GF@expr1_type string@int\n");
        }
    }
    if (data_of_return->node_data->data_type == DTYPE_DOUBLE) {
        printf("TYPE GF@expr1_type LF@ret_value\n");
        if (data_of_return->node_data->can_be_nil != 0) {
            printf(" JUMPIFNEQ __error_sem_type_check__  GF@expr1_type string@float\n");
        }
    }
    if (data_of_return->node_data->data_type == DTYPE_STRING) {
        printf("TYPE GF@expr1_type LF@ret_value\n");
        if (data_of_return->node_data->can_be_nil == 0) {
            printf("JUMPIFNEQ __error_sem_type_check__  GF@expr1_type string@string\n");
        }
    }
    if (data_of_return->node_data->data_type == DTYPE_VOID) {

    }
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL __main_%d__\n", label_num);
}

void inorderTraversal_bst(bst_node_t* root, bool is_local){
    if (root != NULL) {
        inorderTraversal_bst(root->left, is_local);

        // Perform the action on the current node
        if(root->node_data->symbol_type == TYPE_VAR){
            generate_var_definition(root->key, is_local);
        }

        inorderTraversal_bst(root->right, is_local);
    }
}

void gen_func_dec(bst_node_t* root, char* func_id, bool is_local){
    printf("JUMP __%s_return__\n", func_id);
    printf("LABEL __%s_declaration__\n", func_id);
    inorderTraversal_bst(root, is_local);
    printf("JUMP __do_%s__\n", func_id);
}

void generate_var_definition(char* var_id, bool local) {
    if (local)
        printf("DEFVAR LF@%s\n", var_id);
    else
        printf("DEFVAR GF@%s\n", var_id);
}

void gen_func_call(char* func_id) {
    printf("CALL __%s__\n", func_id);
    printf("MOVE GF@tmp_var TF@ret_value\n");
}

void gen_exit_from_func(){
    printf("EXIT int@0\n");
}
void gen_check_expr_types(){
    printf("LABEL __check_expr_types__\n");
    printf("POPS GF@ast_expr1\n");
    printf("TYPE GF@expr1_type GF@ast_expr1\n");
    printf("POPS GF@ast_expr2\n");
    printf("TYPE GF@expr2_type GF@ast_expr2\n");
    printf("JUMPIFEQ __wrong_operand_types__ GF@expr1_type string@bool\n");
    printf("JUMPIFEQ __wrong_operand_types__ GF@expr2_type string@bool\n");
}
void gen_one_operand_expr(Token* token,  bool is_local){
    switch (token->type) {
        case L_INT:
            printf("MOVE GF@tmp_var int@%d\n", token->data.integer);
            break;
        case L_DOUBLE:
            printf("MOVE GF@tmp_var float@%a\n", token->data.double_num);
            break;
        case L_EXP:
            printf("MOVE GF@tmp_var float@%a\n", token->data.double_num);
            break;
        case L_STRING: {
            char *string_to_insert;
            string_to_insert = create_string_4_gen(token->data.stringValue.str);
            printf("MOVE GF@tmp_var string@%s\n", string_to_insert);
            break;
        }
        case L_STRING_PLUS: {
            char *string_plus_to_insert;
            string_plus_to_insert = create_string_4_gen(token->data.stringValue.str);
            printf("MOVE GF@tmp_var string@%s\n", string_plus_to_insert);
            break;
        }
        case L_ID:{
            if(is_local){
                printf("TYPE GF@expr1_type LF@%s\n", token->data.stringValue.str);
                printf("JUMPIFNEQ __defined_local_var__ GF@expr1_type string@\n");
                printf("JUMP __undefined_var__\n");
                printf("LABEL __defined_local_var__\n");
                printf("PUSHS LF@%s\n", token->data.stringValue.str);
                printf("POPS GF@tmp_var\n");
            }else{
                printf("TYPE GF@expr1_type GF@%s\n", token->data.stringValue.str);
                printf("JUMPIFNEQ __defined_global_var__ GF@expr1_type string@\n");
                printf("JUMP __undefined_var__\n");
                printf("LABEL __defined_global_var__\n");
                printf("PUSHS GF@%s\n", token->data.stringValue.str);
                printf("POPS GF@tmp_var\n");
            }
        }
        default:
            break;
    }
}

void gen_ast(bool is_in_func, int* label_num, ast_node_t* current_node){
    if(current_node == NULL)
        return;

    gen_ast(is_in_func, label_num, current_node->left);
    *label_num = *label_num + 1;
    gen_ast(is_in_func, label_num, current_node->right);
    *label_num = *label_num + 1;
    switch (current_node->token->type){
        case L_PLUS:
            gen_check_expr_types();

            printf("JUMPIFEQ __check%d_second_type__ GF@expr1_type string@string\n", *label_num);
            printf("JUMPIFEQ __wrong_operand_types__ GF@expr2_type string@string\n");
            // adding two operand and checking if the second is number
            printf("JUMPIFNEQ __number_type1%d_is_not_nil__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr1\n");
            printf("TYPE GF@expr1_type GF@ast_expr1\n");
            printf("LABEL __number_type1%d_is_not_nil__\n", *label_num);

            printf("JUMPIFNEQ __number_type2%d_is_not_nil__ GF@expr2_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr2\n");
            printf("TYPE GF@expr2_type GF@ast_expr2\n");
            printf("LABEL __number_type2%d_is_not_nil__\n", *label_num);
            //if two numbers have identical types
            printf("JUMPIFEQ __same_types%d__ GF@expr1_type GF@expr2_type\n", *label_num);
            printf("JUMPIFEQ __change_the_1_type%d__ GF@expr1_type string@int\n", *label_num);
            printf("INT2FLOAT GF@ast_expr2 GF@ast_expr2\n");
            printf("JUMP __same_types%d__\n", *label_num);

            printf("LABEL __change_the_1_type__%d\n", *label_num);
            printf("INT2FLOAT GF@ast_expr1 GF@ast_expr1\n");

            printf("LABEL __same_types%d__\n", *label_num);
            printf("ADD GF@tmp_var  GF@ast_expr1 GF@ast_expr2\n");
            printf("JUMP __push%d_result__\n", *label_num);

            // check if the second type expr is string
            printf("LABEL __check%d_second_type__ \n", *label_num);
            printf("JUMPIFNEQ __wrong_operand_types__ GF@expr1_type GF@expr2_type\n");
            // checking if the 1  expression_type  is nil
            printf("JUMPIFNEQ __expr1_type%d_is_not_nil__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS string@\n");
            printf("POPS GF@ast_expr1\n");

            printf("LABEL __expr1_type%d_is_not_nil__\n", *label_num);
            // checking if the 2  expression_type  is nil
            printf("JUMPIFNEQ __expr2_type%d_is_not_nil__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS string@\n");
            printf("POPS GF@ast_expr2\n");

            printf("LABEL __expr2_type%d_is_not_nil__\n", *label_num);
            printf("CONCAT GF@tmp_var GF@ast_expr1 GF@ast_expr2\n");
            printf("JUMP __push%d_result__\n", *label_num);

            printf("LABEL __push%d_result__\n", *label_num);
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_MINUS:
            gen_check_expr_types();

            printf("JUMPIFEQ __wrong_operand_types__ GF@expr1_type string@string\n");
            printf("JUMPIFEQ __wrong_operand_types__ GF@expr2_type string@string\n");
            printf("JUMPIFNEQ __number_type1%d_is_not_nil__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr1\n");
            printf("TYPE GF@expr1_type GF@ast_expr1\n");

            printf("LABEL __number_type1%d_is_not_nil__\n", *label_num);
            printf("JUMPIFNEQ __number_type2%d_is_not_nil__ GF@expr2_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr2\n");
            printf("TYPE GF@expr2_type GF@ast_expr2\n");

            printf("LABEL __number_type2%d_is_not_nil__\n", *label_num);
            printf("JUMPIFEQ __same_types%d__ GF@expr1_type GF@expr2_type\n", *label_num);
            printf("JUMPIFEQ __change_the_1_type%d__ GF@expr1_type string@int\n", *label_num);
            printf("INT2FLOAT GF@ast_expr2 GF@ast_expr2\n");
            printf("JUMP __same_types%d__\n", *label_num);

            printf("LABEL __change_the_1_type__%d\n", *label_num);
            printf("INT2FLOAT GF@ast_expr1 GF@ast_expr1\n");

            printf("LABEL __same_types%d__\n", *label_num);
            printf("SUB GF@tmp_var  GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_MULTIPLY:
            gen_check_expr_types();

            printf("JUMPIFEQ __wrong_operand_types__ GF@expr1_type string@string\n");
            printf("JUMPIFEQ __wrong_operand_types__ GF@expr2_type string@string\n");
            printf("JUMPIFNEQ __number_type1%d_is_not_nil__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr1\n");
            printf("TYPE GF@expr1_type GF@ast_expr1\n");

            printf("LABEL __number_type1%d_is_not_nil__\n", *label_num);
            printf("JUMPIFNEQ __number_type2%d_is_not_nil__ GF@expr2_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr2\n");
            printf("TYPE GF@expr2_type GF@ast_expr2\n");

            printf("LABEL __number_type2%d_is_not_nil__\n", *label_num);
            printf("JUMPIFEQ __same_types%d__ GF@expr1_type GF@expr2_type\n", *label_num);
            printf("JUMPIFEQ __change_the_1_type%d__ GF@expr1_type string@int\n", *label_num);
            printf("INT2FLOAT GF@ast_expr2 GF@ast_expr2\n");
            printf("JUMP __same_types%d__\n", *label_num);

            printf("LABEL __change_the_1_type__%d\n", *label_num);
            printf("INT2FLOAT GF@ast_expr1 GF@ast_expr1\n");

            printf("LABEL __same_types%d__\n", *label_num);
            printf("MUL GF@tmp_var  GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_DIVIDE:
            gen_check_expr_types();

            printf("JUMPIFEQ __wrong_operand_types__ GF@expr1_type string@string\n");
            printf("JUMPIFEQ __wrong_operand_types__ GF@expr2_type string@string\n");
            printf("JUMPIFNEQ __number_type1%d_is_not_nil__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr1\n");
            printf("TYPE GF@expr1_type GF@ast_expr1\n");

            printf("LABEL __number_type1%d_is_not_nil__\n", *label_num);
            printf("JUMPIFNEQ __number_type2%d_is_not_nil__ GF@expr2_type string@nil\n", *label_num);
            printf("JUMP __other_sem_errors__\n");

            printf("LABEL __number_type2%d_is_not_nil__\n", *label_num);
            printf("JUMPIFEQ __same_types%d__ GF@expr1_type GF@expr2_type\n", *label_num);
            printf("JUMPIFEQ __change_the_1_type%d__ GF@expr1_type string@int\n", *label_num);
            printf("INT2FLOAT GF@ast_expr2 GF@ast_expr2\n");
            printf("JUMP __same_types%d__\n", *label_num);

            printf("LABEL __change_the_1_type__%d\n", *label_num);
            printf("INT2FLOAT GF@ast_expr1 GF@ast_expr1\n");

            printf("LABEL __same_types%d__\n", *label_num);
            printf("DIV GF@tmp_var  GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_EQUAL:
            gen_check_expr_types();

            printf("JUMPIFEQ __same_types%d__ GF@expr1_type GF@expr2_type\n", *label_num);
            printf("PUSHS bool@false\n");
            printf("JUMP __not_the_same_types%d__\n", *label_num);

            printf("LABEL __same_types%d__\n", *label_num);
            printf("EQ GF@tmp_var GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            printf("LABEL __not_the_same_types%d__\n", *label_num);
            printf("POPS GF@tmp_var\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_NOT_EQUAL:
            gen_check_expr_types();

            printf("JUMPIFEQ __same_types%d__ GF@expr1_type GF@expr2_type\n", *label_num);
            printf("PUSHS bool@true\n");
            printf("JUMP __not_the_same_types%d__\n", *label_num);

            printf("LABEL __same_types%d__\n", *label_num);
            printf("EQ GF@tmp_var GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            printf("NOTS\n");

            printf("LABEL __not_the_same_types%d__\n", *label_num);
            printf("POPS GF@tmp_var\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_LESS:
            gen_check_expr_types();

            printf("JUMPIFEQ __same_types%d__  GF@expr1_type GF@expr2_type\n", *label_num);
            printf("JUMPIFEQ __change_the_1_type%d__ GF@expr1_type string@int\n", *label_num);
            printf("INT2FLOAT GF@ast_expr2 GF@ast_expr2\n");
            printf("JUMP __same_types%d__\n", *label_num);

            printf("LABEL __change_the_1_type%d__\n", *label_num);
            printf("INT2FLOAT GF@ast_expr1 GF@ast_expr1\n");

            printf("LABEL __same_types%d__\n", *label_num);
            printf("LT GF@tmp_var GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_GREATER:
            gen_check_expr_types();

            printf("JUMPIFEQ __same_types%d__  GF@expr1_type GF@expr2_type\n", *label_num);
            printf("JUMPIFEQ __change_the_1_type%d__ GF@expr1_type string@int\n", *label_num);
            printf("INT2FLOAT GF@ast_expr2 GF@ast_expr2\n");
            printf("JUMP __same_types%d__\n", *label_num);

            printf("LABEL __change_the_1_type%d__\n", *label_num);
            printf("INT2FLOAT GF@ast_expr1 GF@ast_expr1\n");

            printf("LABEL __same_types%d__\n", *label_num);
            printf("GT GF@tmp_var GF@ast_expr1 GF@ast_expr2\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_INT:
            printf("PUSHS int@%d\n", current_node->token->data.integer);
            break;
        case L_STRING: {
            char *string_to_insert;
            string_to_insert = create_string_4_gen(current_node->token->data.stringValue.str);
            printf("PUSHS int@%s\n", string_to_insert);
            break;
        }
        case L_STRING_PLUS:
        {
            char *string_plus_to_insert;
            string_plus_to_insert = create_string_4_gen(current_node->token->data.stringValue.str);
            printf("PUSHS int@%s\n", string_plus_to_insert);
            break;
        }
        case L_DOUBLE:
            printf("PUSHS float@%a\n", current_node->token->data.double_num);
            break;
        case L_EXP:
            printf("PUSHS float@%a\n", current_node->token->data.double_num);
            break;
        case L_ID:
            if(is_in_func == 0){
                printf("TYPE GF@expr1_type GF@%s\n", current_node->token->data.stringValue.str);
                printf("JUMPIFEQ __undefined_var__ GF@expr1_type string@\n");
                printf("PUSHS GF@%s\n", current_node->token->data.stringValue.str);
            }else{
                printf("TYPE GF@expr1_type LF@%s\n", current_node->token->data.stringValue.str);
                printf("JUMPIFEQ __undefined_var__ GF@expr1_type string@\n");
                printf("PUSHS LF@%s\n", current_node->token->data.stringValue.str);
            }
            break;
        case L_NIL_OPERATOR:
            gen_check_expr_types();

            printf("JUMPIFEQ __same_types%d__ GF@expr2_type GF@expr1_type\n", *label_num);
            printf("JUMP __wrong_operand_types__\n");
            printf("LABEL __same_types%d__\n", *label_num);
            printf("JUMPIFEQ __exp1_type%d_is_string GF@expr1_type string@string\n", *label_num);
            printf("JUMPIFEQ __exp1_type%d_is_float GF@expr1_type string@float\n", *label_num);
            printf("JUMPIFEQ __exp1_type%d_is_int GF@expr1_type string@int\n", *label_num);

            printf("LABEL __exp1_type%d_is_float\n", *label_num);
            printf("JUMPIFNEQ __assign%d_1_operand__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS float@0x0p+0\n");
            printf("POPS GF@ast_expr1\n");
            printf("JUMP __assign%d_2_operand__\n", *label_num);

            printf("LABEL __exp1_type%d_is_int\n", *label_num);
            printf("JUMPIFNEQ __assign%d_1_operand__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS int@0\n");
            printf("POPS GF@ast_expr1\n");
            printf("JUMP __assign%d_2_operand__\n", *label_num);

            printf("LABEL __exp1_type%d_is_string\n", *label_num);
            printf("JUMPIFNEQ __assign%d_1_operand__ GF@expr1_type string@nil\n", *label_num);
            printf("PUSHS string@\n");
            printf("POPS GF@ast_expr1\n");
            printf("JUMP __assign%d_2_operand__\n", *label_num);

            printf("LABEL __assign%d_2_operand__\n", *label_num);
            printf("PUSHS GF@ast_expr2\n");
            printf("JUMP __push%d_result__\n", *label_num);

            printf("LABEL __assign%d_1_operand__\n", *label_num);
            printf("PUSHS GF@ast_expr1\n");
            printf("JUMP __push%d_result__\n", *label_num);

            printf("LABEL __push%d_result__\n", *label_num);
            printf("POPS GF@tmp_var\n");
            printf("PUSHS GF@tmp_var\n");
            break;


        case L_ID_EXMARK:
            printf("POPS GF@ast_expr1\n");
            printf("TYPE GF@expr1_type GF@ast_expr1\n");
            printf("JUMPIFEQ __wrong_operand_types__ GF@expr1_type string@bool\n");
            printf("JUMPIFEQ __wrong_operand_types__ GF@expr1_type string@nil\n");
            printf("MOVE GF@tmp_var GF@ast_expr1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        default:
            break;
    }
}
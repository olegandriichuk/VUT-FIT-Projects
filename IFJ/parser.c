/**
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 * 
 * @brief Syntax/semantic analysis implementation
 */

#include "parser.h"
#include "codegen.h"
#include <stdbool.h>

int counted_args;
Token token;
Stack main_stack;
StringValue current_function;
bool inside_ifwhile = false;
bool inside_function = false;
int label_num = 0;
int function_label_num = 0;
bool didnt_visited_retun = true;


typedef struct
{
    /** Pole pro uložení hodnot. */
    Token **array;
    /** Index prvku na vrcholu zásobníku. */
    int buffer;
    int size;
    int counter;
} token_array;

token_array main_array;

void counter_dec()
{
    main_array.counter--;
}

void starting_routine()
{
    strInit(&current_function);
    Stack_Init(&main_stack);
    Stack_Push(&main_stack);
    array_init();
    save_tokens();
    parse_for_func();
    add_builtin_func();
}

void save_tokens()
{
    do
    {
        store_to_array();
    } while (main_array.array[main_array.size - 1]->type != L_EOF);
}

void parse()
{
    
    gen_head();
    starting_routine();
    main_array.counter = -1;

    newToken();

    program();
    return;
}
void program()
{


    if (token.type == L_EOF)
    {
            gen_func_dec(main_stack.array[0], "main", false); 
            gen_end_of_main();
        
        strFree(&current_function);
        Stack_Pop(&main_stack);
        return;
    }
    else
    {
        statement();
    }
    program();
}

void parse_for_func()
{
    do
    {
        newToken();
        if (token.data.keyword == K_FUNC && token.type == L_KEYWORD)
        {
            type_check(L_ID, "ID expected 1", true);
            node_data new_node;
            StringValue id;
            create_new_node_and_str(&new_node, &id, false, 0, TYPE_FNC, DTYPE_VOID);

            if (element_is_declared(&main_stack, id.str))
            {
                exit_program(3, "Redefinition of function");
            }

            type_check(L_LBRACKET, "( expected 1", true);
            newToken();
            if (token.type == L_ID)
            {
                do
                {

                    type_check(L_ID, "Name expected", false);

                    func_params for_copy;
                    strInit(&for_copy.name);
                    strCopy(&token.data.stringValue, &for_copy.name);

                    type_check(L_ID, "ID expected 2", true);
                    type_check(L_COLON, ": expected", true);

                    type_check(L_KEYWORD, "Type expected", true); // <type>
                    if (token.data.keyword != K_INT && token.data.keyword != K_DOUBLE &&
                        token.data.keyword != K_STRING)
                    {
                        exit_program(2, "Invalid type");
                    }
                    update_type_params(&for_copy, token.data.keyword);
                    add_new_element(&new_node.params, for_copy);

                    newToken();
                    new_node.param_number += 1;
                    if (token.type == L_RBRACKET)
                    { 
                        break;
                    }
                    type_check(L_COMMA, "Comma expected", false);
                    newToken();
                } while (true);
            }
            newToken();
            if (token.type == L_ARROW)
            { // ->

                newToken(); // <type>
                if (token.data.keyword != K_INT && token.data.keyword != K_DOUBLE && token.data.keyword != K_STRING)
                {
                    
                    exit_program(2, "Type expected");
                }
                if (token.type == L_KEYWORD_QMARK)
                    new_node.can_be_nil = true;
                update_type(&new_node, token.data.keyword);
                newToken();
            }
            else
            { // without arrow and type
                if (token.type == L_KEYWORD)
                {
                    
                    exit_program(2, "arrow excepted");
                }
                new_node.can_be_nil = true;
                update_type(&new_node, token.data.keyword);
            }
            bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
        }

    } while (token.type != L_EOF);
}

/**
 * @brief Handling func. declare, while, if, var/let id, id, return
 *
 */
void statement()
{

    
    if (token.data.keyword == K_FUNC && token.type == L_KEYWORD)
    { 

        label_num += 1;
        function_label_num = label_num;
        
        inside_function = true;
        if (inside_ifwhile)
        {
            exit_program(2, "Function cannot be declared in while or if");
        }

        type_check(L_ID, "ID expected 3", true); // ID

        check_if_token_declared();

        strCopy(&token.data.stringValue, &current_function);

        generate_func_header(current_function.str,function_label_num);
        



        type_check(L_LBRACKET, "( expected", true); // (

        newToken();
        Stack_Push(&main_stack);
        inside_function = true;

        if (token.type == L_ID)
        { // Parsing params
            params();
        }

        type_check(L_RBRACKET, ") expected", false); // )

        newToken();
        if (token.type == L_ARROW)
        {               // ->
            newToken(); // <type>
            if (token.data.keyword != K_INT && token.data.keyword != K_DOUBLE && token.data.keyword != K_STRING)
            {
                exit_program(2, "Type expected");
            }
            newToken();
        }
        else
        { // without arrow and type
            if (token.type == L_KEYWORD)
            {
                exit_program(2, "arrow excepted");
            }
        }

        type_check(L_CURL_LBRACKET, "{ expected", false); // {

        newToken();
        if (token.data.keyword == K_FUNC && token.type == L_KEYWORD)
        {
            exit_program(2, "Function cannot be declared in function");
        }

        
        statement(); // statement
        inside_function = false;
        Stack_Pop(&main_stack);
        

        type_check(L_CURL_RBRACKET, "} expected", false); //}

         bst_node_t* tmp_tree = element_is_declared(&main_stack, current_function.str);
        
        gen_func_dec(main_stack.array[main_stack.topIndex], current_function.str, false);
        
        
        if(tmp_tree->node_data->data_type != DTYPE_VOID && didnt_visited_retun){
            exit_program(6,"Return expected");
        }
        
        newToken();
        inside_function = false;

        statement();
    }

    
    if (token.data.keyword == K_WHILE && token.type == L_KEYWORD)
    { // while
        label_num += 1;
        
        
        inside_ifwhile = true;
        exp_check(NULL, false, NULL);

        type_check(L_CURL_LBRACKET, "{ expected", false); // {

        newToken();

        
        Stack_Push(&main_stack);
        inside_ifwhile = true;
        statement(); // <statement>
        inside_ifwhile = false;
        Stack_Pop(&main_stack);
        

        type_check(L_CURL_RBRACKET, "} expected", false); // }

    
        newToken();

        inside_ifwhile = false;
        
        statement();
        return;
    }

    if (token.data.keyword == K_IF && token.type == L_KEYWORD)
    {
        label_num += 1;
        
        /*                   -->IF<--                       */
        inside_ifwhile = true;

        newToken();
        if (token.type == L_KEYWORD && token.data.keyword == K_LET)
        {
            type_check(L_ID, "Id expected 4", true);
            check_if_token_declared();

            type_check(L_CURL_LBRACKET, "{ expected", true);

            newToken();
            Stack_Push(&main_stack);
            inside_ifwhile = true;
            statement();
            inside_ifwhile = false;
            Stack_Pop(&main_stack);
            
            
            type_check(L_CURL_RBRACKET, "} expected", false);
            
            newToken();
            if (token.data.keyword != K_ELSE && token.type != L_KEYWORD)
            {

                exit_program(2, "Else expected");
            }

            type_check(L_CURL_LBRACKET, "{ expected", true); 

            newToken();
            if (token.data.keyword == K_FUNC && token.type == L_KEYWORD)
            {
                exit_program(2, "Function cannot be declared in while or if");
            }

            Stack_Push(&main_stack);
            inside_ifwhile = true;
            statement();
            inside_ifwhile = false;
            Stack_Pop(&main_stack);

            type_check(L_CURL_RBRACKET, "} expected", false); 

            newToken(); // CHECK
            
            
            statement();
            return;
        }


        exp_check(main_array.array[main_array.counter], false, NULL);

      

        type_check(L_CURL_LBRACKET, "{ expected", false); // {

        newToken();

        Stack_Push(&main_stack);
        inside_ifwhile = true;
        statement();
        inside_ifwhile = false;
        Stack_Pop(&main_stack);

        type_check(L_CURL_RBRACKET, "} expected", false); // }

        /*                   -->IF<--                       */

        /*                  -->ELSE<--                      */
        newToken();
        if (token.data.keyword != K_ELSE && token.type != L_KEYWORD)
        {

            exit_program(2, "Else expected");
        }

        type_check(L_CURL_LBRACKET, "{ expected", true); // {

        newToken();

        Stack_Push(&main_stack);
        inside_ifwhile = true;
        statement();
        inside_ifwhile = false;
        Stack_Pop(&main_stack);

        type_check(L_CURL_RBRACKET, "} expected", false); // }

        

        /*                  -->ELSE<--                      */

        newToken();

        inside_ifwhile = false;


        statement();
        return;
    }

    if (token.type == L_KEYWORD && token.data.keyword == K_VAR)
    {
        bool type_or_assign = false; 

        newToken();

        if (token.type != L_ID || token.type == L_KEYWORD || !(strcmp(token.data.stringValue.str, "_")))
        {
            exit_program(2, "Var -> ID is either missing or it's a keyword)");
        }

        node_data new_node;
        StringValue id;
        create_new_node_and_str(&new_node, &id, false, 0, TYPE_VAR, DTYPE_VOID);
        if (element_is_declared_in_local_scope(&main_stack, id.str))
            exit_program(3, "Trying to redefine the variable");

        newToken();
        if (token.type == L_COLON)
        { // : <type>
            newToken();
            if ((token.data.keyword != K_INT && token.data.keyword != K_DOUBLE && token.data.keyword != K_STRING))
            {
                exit_program(2, "Type expected");
            }
            if (token.type == L_KEYWORD_QMARK)
                new_node.can_be_nil = true;
            update_type(&new_node, token.data.keyword);
            type_or_assign = true;
            newToken();
        }
        else
        {
            if (token.data.keyword == K_INT || token.data.keyword == K_DOUBLE || token.data.keyword == K_STRING)
            {

                exit_program(2, "Colon expected");
            }
        }

        if (token.type == L_ASSIGN)
        { // var x = ....
            newToken();
            

            if ((token.type == L_KEYWORD && token.data.keyword != K_NIL) || (token.type != L_INT && token.type != L_ID && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE && token.data.keyword != K_NIL && token.type != L_EXP && token.type != L_LBRACKET))
            {
                exit_program(2, "Missing or Invalid assign value");
            }

            type_or_assign = true;
            if (token.type == L_LBRACKET)
            {
                exp_check(main_array.array[main_array.counter], true, &new_node); // var x = ( 1 + ...
                counter_dec();
                newToken();
            }
            if (token.type == L_INT || token.type == L_DOUBLE || token.type == L_STRING || token.type == L_STRING_PLUS || token.data.keyword == K_NIL || token.type == L_EXP)
            { // var x = 1 || var x = 1.1 || var x = "Ahoj"

                if (new_node.data_type != DTYPE_VOID)
                {
                    if (not_compatible_nd(&new_node, token.type))
                    {
                        exit_program(7, "Trying to assign from value of incompatible type (var = smth)");
                    }
                    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
                }
                else
                {
                    update_type_t(&new_node, token.type);
                    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
                    type_or_assign = true;
                }

                exp_check(main_array.array[main_array.counter], true, &new_node); // var x = 1 + ....
                counter_dec();

                newToken();
            }
            else if (token.type == L_ID)
            {

                type_or_assign = true;
                

                check_if_token_declared();
                StringValue id2;
                strInit(&id2);
                strCopy(&token.data.stringValue, &id2);
                bst_node_t *node_of_id2 = element_is_declared(&main_stack, id2.str);
                if (new_node.data_type != DTYPE_VOID)
                {
                    if (new_node.data_type != node_of_id2->node_data->data_type)
                        exit_program(7, "Trying to assign from value of incompatible type (var = id)");
                }
                else
                {
                    new_node.data_type = node_of_id2->node_data->data_type;
                }
                bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
                node_of_id2 = element_is_declared(&main_stack, id2.str);

                if (node_of_id2)
                {
                    if (node_of_id2->node_data->symbol_type == TYPE_VAR)
                    {
                        
                        exp_check(main_array.array[main_array.counter], true, &new_node); // var x = a + ....
                        counter_dec();
                    }

                    if (node_of_id2->node_data->symbol_type == TYPE_FNC)
                    {
                        
                        counted_args = 0;
                        fnc_call(id2.str);
                        if (node_of_id2->node_data->param_number < counted_args && node_of_id2->node_data->param_number != -1)
                            exit_program(4, "Too many parameters in function call");
                        else if (node_of_id2->node_data->param_number > counted_args)
                            exit_program(4, "Few parameters in function call");
                        newToken();
                        statement();
                        return;
                    }
                }

                newToken();
            }
        }
        else
        {
            if (type_or_assign)
            {
                bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
            }
        }

        if (!(type_or_assign))
        { // Declaration cant be just var x
            exit_program(2, "Declaration wihout type or value assign");
        }

        didnt_visited_retun = false;
        statement();
    }

    
    if (token.type == L_KEYWORD && token.data.keyword == K_LET)
    { 

        bool type_or_assign = false; // Checking -> var a eps

        

        newToken();

        

        if (token.type != L_ID || token.type == L_KEYWORD || !(strcmp(token.data.stringValue.str, "_")))
        {
            exit_program(2, "Let -> ID is either missing or it's a keyword)");
        }

        node_data new_node;
        StringValue id;
        create_new_node_and_str(&new_node, &id, false, 0, TYPE_LET, DTYPE_VOID);
        if (element_is_declared_in_local_scope(&main_stack, id.str))
            exit_program(3, "Trying to redefine the variable");

        newToken();
        if (token.type == L_COLON)
        { // : <type>
            newToken();
            if (token.data.keyword != K_INT && token.data.keyword != K_DOUBLE && token.data.keyword != K_STRING)
            {
                exit_program(2, "Type expected");
            }
            if (token.type == L_KEYWORD_QMARK)
                new_node.can_be_nil = true;
            update_type(&new_node, token.data.keyword);

            type_or_assign = true;
            newToken();
        }
        else
        {
            if (token.data.keyword == K_INT || token.data.keyword == K_DOUBLE || token.data.keyword == K_STRING)
            {

                exit_program(2, "Colon expected");
            }
        }

        if (token.type == L_ASSIGN)
        { // let x  = ....
            newToken();
            

            if ((token.type == L_KEYWORD && token.data.keyword != K_NIL) || (token.type != L_INT && token.type != L_ID && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE && token.data.keyword != K_NIL && token.type != L_EXP && token.type != L_LBRACKET))
            {
                exit_program(2, "Missing or Invalid assign value");
            }

            type_or_assign = true;

            if (token.type == L_LBRACKET)
            {
                exp_check(main_array.array[main_array.counter], true, &new_node); // let x = ( 1 + ...
                counter_dec();
                newToken();
            }

            if (token.type == L_INT || token.type == L_DOUBLE || token.type == L_STRING || token.type == L_STRING_PLUS || token.data.keyword == K_NIL || token.type == L_EXP)
            { // let x = 1 || let x = 1.1 || let x = "Ahoj"
                if (new_node.data_type != DTYPE_VOID)
                {
                    if (not_compatible_nd(&new_node, token.type))
                    {
                        exit_program(7, "Trying to assign from value of incompatible type(var = smth)");
                    }
                    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
                }
                else
                {
                    update_type_t(&new_node, token.type);
                    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
                    type_or_assign = true;
                }
                exp_check(main_array.array[main_array.counter], true, &new_node);
                counter_dec();

                newToken();
            }
            else if (token.type == L_ID)
            {
                type_or_assign = true;
                check_if_token_declared();
                StringValue id2;
                strInit(&id2);
                strCopy(&token.data.stringValue, &id2);
                bst_node_t *node_of_id2 = element_is_declared(&main_stack, id2.str);

                if (new_node.data_type != DTYPE_VOID)
                {
                    if (new_node.data_type != node_of_id2->node_data->data_type)
                        exit_program(7, "Trying to assign from value of incompatible type (var = id)");
                }
                else
                {
                    new_node.data_type = node_of_id2->node_data->data_type;
                }
                bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
                node_of_id2 = element_is_declared(&main_stack, id2.str);

                if (node_of_id2)
                {

                    if (node_of_id2->node_data->symbol_type == TYPE_FNC)
                    {
                        counted_args = 0;
                        fnc_call(id2.str);
                        if (node_of_id2->node_data->param_number < counted_args && node_of_id2->node_data->param_number != -1)
                            exit_program(4, "Too many parameters in function call");
                        else if (node_of_id2->node_data->param_number > counted_args)
                            exit_program(4, "Few parameters in function call");
                        newToken();
                        statement();

                        return;
                    }
                    if (node_of_id2->node_data->symbol_type == TYPE_LET)
                    {
                        exp_check(main_array.array[main_array.counter], true, &new_node);
                        counter_dec();
                    }
                }

                newToken();
            }
        }
        else
        {
            if (type_or_assign)
            {
                bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
            }
        }

        if (!(type_or_assign))
        { // Declaration cant be just let x
            exit_program(2, "Declaration wihout type or value assign");
        }

        statement();
    }

    if (token.type == L_KEYWORD && token.data.keyword == K_RETURN)
    {
        bst_node_t *curr_function = element_is_declared(&main_stack, current_function.str);

        if (!inside_function)
        {
            exit_program(2, "Return outside function");
        }

        if (!curr_function->node_data->can_be_nil)
        {
            
            newToken();
            if (token.type == L_INT || token.type == L_DOUBLE || token.type == L_STRING || token.type == L_EXP || token.type == L_STRING_PLUS)
            {
                if (not_compatible(curr_function, token.type))
                    exit_program(4, "Wrong return type (return smth)");
                
                exp_check(main_array.array[main_array.counter], true, curr_function->node_data);
                counter_dec();
                
            }
            else if (token.type == L_ID)
            {
                check_if_token_declared();
                StringValue id;
                strInit(&id);
                strCopy(&token.data.stringValue, &id);

                bst_node_t *return_value = element_is_declared(&main_stack, id.str);

                if (!return_value)
                    exit_program(5, "Return value isn't declared");

                if (curr_function->node_data->data_type != return_value->node_data->data_type)
                    exit_program(4, "Returning variable of a wrong type");

                exp_check(main_array.array[main_array.counter], true, curr_function->node_data);
                counter_dec();

                strFree(&id);
            }
            else
            {
                exit_program(6, "Missing return value");
            }
            newToken();
        }
        else
        {
            newToken();
            if (token.type == L_ID)
            {
                exit_program(6, "Trying to return value from function with nil type");
            }
        }

    
        if (token.type == L_KEYWORD && token.data.keyword == K_RETURN)
        {

            exit_program(2, "2 or more returns in a row");
        }

        
        didnt_visited_retun = false;
        gen_func_return(function_label_num, curr_function);

        gen_exit_from_func();

        statement();
    }

    
    if (token.type == L_ID)
    {

        StringValue id;
        strInit(&id);
        strCopy(&token.data.stringValue, &id);
        bst_node_t *variable = element_is_declared(&main_stack, id.str);

        if(variable){
            if (variable->node_data->symbol_type == TYPE_FNC){
                counted_args = 0;
                fnc_call(id.str);
                if (!variable)
                    exit_program(3, "Function isn't defined");
                if (variable->node_data->symbol_type != TYPE_FNC)
                    exit_program(9, "Calling id as function");
                if (variable->node_data->param_number < counted_args && variable->node_data->param_number != -1)
                    exit_program(4, "Too many parameters in function call");
                else if (variable->node_data->param_number > counted_args)
                    exit_program(4, "Few parameters in function call");
                
                if(strcmp(variable->key,"write") != 0){
                    gen_func_call(variable->key);
                }
            
            
            
            
            newToken();
            statement();
            return;
            }
        }else{
            exit_program(3,"Undefined function");
        }

        newToken();

        if (token.type == L_LBRACKET)
        { // If true -> function()
            
            fnc_call(id.str);
            if (!variable)
            {
                exit_program(3, "Funtion isnt defined");
            }
            

            newToken();
            statement();
            return;
        }

        if (!variable)
        {
            exit_program(5, "Variable isnt defined");
        }

        if (token.type == L_ASSIGN)
        {
            if (variable->node_data->symbol_type == TYPE_LET)
                exit_program(9, "Trying to change const");

            newToken();

            if ((token.type == L_KEYWORD && token.data.keyword != K_NIL) || (token.type != L_INT && token.type != L_ID && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE && token.data.keyword != K_NIL && token.type != L_LBRACKET))
            {
                exit_program(2, "Missing or Invalid assign value");
            }

            if (token.type == L_LBRACKET)
            {
                exp_check(main_array.array[main_array.counter], true, variable->node_data); // x = ( 1 + ...
                counter_dec();
                newToken();
            }


            if (token.type == L_INT || token.type == L_DOUBLE || token.type == L_STRING || token.type == L_STRING_PLUS || token.data.keyword == K_NIL || token.type == L_EXP)
            { // x = 1 || x = 1.1 || x = "Ahoj"
                if (not_compatible(variable, token.type))
                    exit_program(7, "Assigning from a wrong type");

                exp_check(main_array.array[main_array.counter], true, variable->node_data);
                counter_dec();

                newToken();
                statement();

                return;
            }
            if (token.type == L_ID)
            { // id = x || id = write()

                check_if_token_declared();
                StringValue id2;
                strInit(&id2);
                strCopy(&token.data.stringValue, &id2);
                bst_node_t *variable_second = element_is_declared(&main_stack, id2.str);
                if (variable->node_data->data_type != variable_second->node_data->data_type)
                    exit_program(7, "Trying to assign from a variable or func with incompatible return type");

                if (variable_second)
                {

                    if (variable_second->node_data->symbol_type == TYPE_FNC)
                    {
                        
                        counted_args = 0;
                        fnc_call(id2.str);
                        if (variable_second->node_data->param_number < counted_args && variable_second->node_data->param_number != -1)
                            exit_program(4, "Too many parameters in function call");
                        else if (variable_second->node_data->param_number > counted_args)
                            exit_program(4, "Few parameters in function call");
                        newToken();
                        statement();
                        return;
                    }
                    if (variable_second->node_data->symbol_type == TYPE_VAR)
                    {
                        
                        exp_check(main_array.array[main_array.counter], true, variable_second->node_data);
                        counter_dec();
                    }
                }

                newToken();

            }
        }

        else
        {
            exit_program(2, "ID cant be solo");
        }
        statement();
    }

    statement_check(token);
}

void exp_check(Token *used_token, bool is_expression, node_data *new_node)
{
    ast_node_t *tree;
    int err_code;
    err_code = expression(used_token, is_expression, new_node, &main_stack, &tree);
    
    switch (err_code)
    {
    case 0:
        if(is_expression == false && ast_tree_height(tree) == 1){
            gen_one_operand_expr(tree->token,inside_function);   
        }
        else {
            label_num += 1;
            gen_ast(inside_function,&label_num,tree);
        }
        break;

    case 2:
        exit_program(2, "Expression failed\n");
        break;
    case 5:
        exit_program(5, "Undefined variable\n");
        break;
    case 7:
        exit_program(7, "Invalid type in expression\n");
        break;

    default:
        break;
    }
}

/**
 * @brief Function call
 *
 * @param fnc_name
 */
void fnc_call(char *fnc_name)
{
    
    type_check(L_LBRACKET, "( expected", true);

    newToken();
    if (token.type == L_RBRACKET)
    {
        
        return;
    }
    args(fnc_name);
}

/**
 * @brief Arguments for function call
 *
 */
void args(char* curr)
{
    bst_node_t *curr_function = element_is_declared(&main_stack, curr);
    if (curr_function->node_data->param_number == 0) {
        exit_program(4, "This function doesnt have args");
    }

    if (token.type != L_ID && token.type != L_INT && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE)
    {

        exit_program(2, "Invalid argument");
    }

    int param_num = curr_function->node_data->param_number;
    StringValue id;
    strInit(&id);
    if (token.type == L_ID)
    { // name
        strCopy(&token.data.stringValue, &id);
        newToken();
        if (token.type == L_COLON)
        {
            if(param_num != -1 ){
                if (strcmp(id.str, curr_function->node_data->params->name.str) != 0) {
                    exit_program(4, "Calling function with wrong name of the parameter");
                }
            }
            newToken();
            if (token.type != L_INT && token.type != L_ID && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE)
            {

                exit_program(2, "Invalid argument value");
            }else if (token.type == L_INT || token.type == L_STRING || token.type == L_DOUBLE) {
                if(param_num != -1 ){
                    if (not_compatible_param(curr_function->node_data->params, token.type)) {
                        exit_program(4, "4Passing parameter of incompatible type");
                    }
                }
            } else if (token.type == L_ID) {
                check_if_token_declared();
                StringValue id2;
                strInit(&id2);
                strCopy(&token.data.stringValue, &id2);
                bst_node_t *just_id = element_is_declared(&main_stack, id2.str);
                if(param_num != -1 ){
                    if (just_id->node_data->data_type != curr_function->node_data->params->type)
                        exit_program(4, "5Passing parameter of incompatible type");
                }
            }
            newToken();
        }else{
            if(token.type == L_ID){
                exit_program(2, "You probably missed the \":\" sign");
            }

            if (!element_is_declared(&main_stack, id.str))
                exit_program(5, "Variable isn't defined");
            if(param_num != -1 ){
            if (strcmp("_", curr_function->node_data->params->name.str) != 0)
                exit_program(4, "Calling function without using parameter name");
            bst_node_t *just_id = element_is_declared(&main_stack, id.str);
            if (just_id->node_data->data_type != curr_function->node_data->params->type)
                exit_program(4, "6Passing parameter of incompatible type");
            }
        }        
    }else{
        if(param_num != -1 ){
            if (strcmp("_", curr_function->node_data->params->name.str) != 0)
                exit_program(4, "Calling function without using parameter name");
        }
    }

    if (token.type == L_INT || token.type == L_STRING || token.type == L_DOUBLE || token.type == L_STRING_PLUS)
    {

        newToken();
    }

    counted_args += 1;
    if (token.type != L_RBRACKET)
    {
        if(param_num != -1 ){
            if (curr_function->node_data->params->next == NULL)
                return;
            }
            args_n(curr_function->node_data->params->next,param_num);
            return;
    }

    return;
}

void args_n(func_params *param, int param_num)
{

    type_check(L_COMMA, ", expected", false);

    newToken();
    if (token.type != L_ID && token.type != L_INT && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE)
    {
        exit_program(2, "Invalid argument");
    }


    StringValue id;
    strInit(&id);
    if (token.type == L_ID)
    { // name
        strCopy(&token.data.stringValue, &id);
        newToken();
        if (token.type == L_COLON)
        {
            if(param_num != -1 ){
                if (strcmp(id.str, param->name.str) != 0) {
                    exit_program(4, "Calling function with wrong name of the parameter");
                }
            }
            newToken();
            if (token.type != L_INT && token.type != L_ID && token.type != L_STRING && token.type != L_STRING_PLUS && token.type != L_DOUBLE)
            {

                exit_program(2, "Invalid argument value");
            }else if (token.type == L_INT || token.type == L_STRING || token.type == L_DOUBLE) {
                if(param_num != -1 ){
                    if (not_compatible_param(param, token.type)) {
                        exit_program(4, "4Passing parameter of incompatible type");
                    }
                }
            } else if (token.type == L_ID) {
                check_if_token_declared();
                StringValue id2;
                strInit(&id2);
                strCopy(&token.data.stringValue, &id2);
                bst_node_t *just_id = element_is_declared(&main_stack, id2.str);
                if(param_num != -1 ){
                    if (just_id->node_data->data_type != param->type)
                        exit_program(4, "5Passing parameter of incompatible type");
                }
            }
            newToken();
        }else{
            if(token.type == L_ID){
                
                exit_program(2, "You probably missed the \":\" sign");
            }

            if (!element_is_declared(&main_stack, id.str))
                exit_program(5, "Variable isn't defined");
            if(param_num != -1 ){
            if (strcmp("_", param->name.str) != 0)
                exit_program(4, "Calling function without using parameter name");
            bst_node_t *just_id = element_is_declared(&main_stack, id.str);
            if (just_id->node_data->data_type != param->type)
                exit_program(4, "6Passing parameter of incompatible type");
            }
        }        
    }else{
        if(param_num != -1 ){
            if (strcmp("_", param->name.str) != 0)
                exit_program(4, "Calling function without using parameter name");
        }
    }

    if (token.type == L_INT || token.type == L_STRING || token.type == L_DOUBLE || token.type == L_STRING_PLUS)
    {

        newToken();
    }

    
    counted_args += 1;
    if (token.type != L_RBRACKET) {
        if(param_num != -1){
            if (param->next->name.str == NULL){
                counted_args += 1;
                return;
            }
        }
        if(param_num != -1){
            args_n(param->next,param_num);
        }else{
            args_n(NULL,param_num);
        }
        return;
        }


    

    return;
}

void params()
{
    
    

    type_check(L_ID, "Name expected", false);
    type_check(L_ID, "ID expected", true);
    StringValue id;
    node_data new_node;
    create_new_node_and_str(&new_node, &id, false, 0, TYPE_VAR, DTYPE_VOID);

    type_check(L_COLON, ": expected", true);

    type_check(L_KEYWORD, "Type expected", true); // <type>
    if (token.data.keyword != K_INT && token.data.keyword != K_DOUBLE && token.data.keyword != K_STRING)
    {

        exit_program(2, "Invalid type");
    }

    update_type(&new_node, token.data.keyword);
    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);

    newToken();
    if (token.type == L_RBRACKET)
    { 

        return;
    }
    
    params_n();
}

void params_n()
{
    
    type_check(L_COMMA, ",", false);

    type_check(L_ID, "Name expected", true);
    type_check(L_ID, "ID expected", true);
    StringValue id;
    node_data new_node;
    create_new_node_and_str(&new_node, &id, false, 0, TYPE_VAR, DTYPE_VOID);
    type_check(L_COLON, ": expected", true);

    type_check(L_KEYWORD, "Type expected", true); // <type>
    if (token.data.keyword != K_INT && token.data.keyword != K_DOUBLE && token.data.keyword != K_STRING)
    {

        exit_program(2, "Invalid type");
    }

    update_type(&new_node, token.data.keyword);
    newToken();
    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);

    if (token.type == L_RBRACKET)
    { 
        return;
    }
    params_n();
}


void statement_check(Token token)
{
    
    if (
        (token.type == L_KEYWORD &&
         (token.data.keyword != K_WHILE && token.data.keyword != K_IF &&
          token.data.keyword != K_LET && token.data.keyword != K_VAR)) ||
        token.type == L_PLUS || token.type == L_MINUS || token.type == L_DIVIDE || token.type == L_MULTIPLY || token.type == L_NIL_OPERATOR
        || token.type == L_INT || token.type == L_DOUBLE || token.type == L_STRING || token.type == L_STRING_PLUS || token.type == L_EXP)
    {
        exit_program(2, "Invalid statement");
    }
}

void create_new_node_and_str(node_data *new_node, StringValue *str, bool nil, int param, id_type s_type,
                             id_data_type d_type)
{
    strInit(str);
    strCopy(&token.data.stringValue, str);

    new_node->can_be_nil = nil;
    new_node->param_number = param;
    new_node->symbol_type = s_type;
    new_node->data_type = d_type;
    new_node->params = NULL;
    
}

void store_to_array()
{
    if (main_array.size == main_array.buffer)
    {

        main_array.array = realloc(main_array.array, main_array.buffer * 2 * sizeof(Token *));
        if (main_array.array == NULL)
            exit_program(99, "Realloc for array failed");
        main_array.buffer *= 2;
    }
    main_array.array[main_array.size] = getToken(file);
    main_array.size += 1;
}

void check_if_token_declared()
{
    StringValue id;
    strInit(&id);
    strCopy(&token.data.stringValue, &id);
    if (!element_is_declared(&main_stack, id.str))
    {
        exit_program(5, "Function or id isn't declared");
    }

    strFree(&id);
}

void array_init()
{
    main_array.array = malloc(sizeof(Token *));
    if (main_array.array == NULL)
        exit_program(99, "Malloc for array_for_tokens didn't go well");
    main_array.buffer = 1;
    main_array.size = 0;
    main_array.counter = -1;
}

Token *exp_token()
{
    newToken();
    return main_array.array[main_array.counter];
}

void newToken()
{
    main_array.counter += 1;
    token = *main_array.array[main_array.counter];
    
}

void type_check(Types type, char *err_msg, bool newTok)
{
    if (newTok)
    {
        newToken();
    }

    if (token.type != type)
    {

        exit_program(2, err_msg);
    }
}

void add_builtin_func()
{
    node_data new_node;
    // func readString() -> String?
    StringValue id;
    strInit(&id);
    id.str = malloc(sizeof(char) * 11);
    id.buffer = 11;
    id.length = 11;
    strcpy(id.str, "readString");
    new_node.data_type = DTYPE_STRING;
    new_node.can_be_nil = true;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 0;
    new_node.params = malloc(sizeof(func_params));
    bst_insert(&main_stack.array[main_stack.topIndex], id.str, new_node);
    // func readInt() -> Int?
    StringValue id2;
    strInit(&id2);
    id.str = malloc(sizeof(char) * 8);
    id.buffer = 8;
    id.length = 8;
    strcpy(id2.str, "readInt");
    new_node.data_type = DTYPE_INT;
    new_node.can_be_nil = true;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 0;
    new_node.params = malloc(sizeof(func_params));
    bst_insert(&main_stack.array[main_stack.topIndex], id2.str, new_node);
    // func readDouble() -> Double?
    StringValue id3;
    strInit(&id3);
    id.str = malloc(sizeof(char) * 11);
    id.buffer = 11;
    id.length = 11;
    strcpy(id3.str, "readDouble");
    new_node.data_type = DTYPE_DOUBLE;
    new_node.can_be_nil = true;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 0;
    new_node.params = malloc(sizeof(func_params));
    bst_insert(&main_stack.array[main_stack.topIndex], id3.str, new_node);
    // func write(1...n)
    StringValue id4;
    strInit(&id4);
    id.str = malloc(sizeof(char) * 6);
    id.buffer = 6;
    id.length = 6;
    strcpy(id4.str, "write");
    new_node.data_type = DTYPE_VOID;
    new_node.can_be_nil = false;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = -1;
    new_node.params = malloc(sizeof(func_params));
    bst_insert(&main_stack.array[main_stack.topIndex], id4.str, new_node);
    // func Int2Double(_ term ∶ Int) -> Double
    StringValue id5;
    strInit(&id5);
    id5.str = malloc(sizeof(char) * 11);
    id5.buffer = 11;
    id5.length = 11;
    strcpy(id5.str, "Int2Double");
    new_node.data_type = DTYPE_DOUBLE;
    new_node.can_be_nil = false;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 1;
    new_node.params = malloc(sizeof(func_params));
    new_node.params->name.str = "_";
    new_node.params->type = DTYPE_INT;
    bst_insert(&main_stack.array[main_stack.topIndex], id5.str, new_node);
    // func Double2Int(_ term ∶ Double) -> Int
    StringValue id6;
    strInit(&id6);
    id.str = malloc(sizeof(char) * 11);
    id.buffer = 11;
    id.length = 11;
    strcpy(id6.str, "Double2Int");
    new_node.data_type = DTYPE_INT;
    new_node.can_be_nil = false;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 1;
    new_node.params = malloc(sizeof(func_params));
    new_node.params->name.str = NULL;
    new_node.params->type = DTYPE_DOUBLE;
    bst_insert(&main_stack.array[main_stack.topIndex], id6.str, new_node);
    // func length(_ 𝑠 : String) -> Int
    StringValue id7;
    strInit(&id7);
    id.str = malloc(sizeof(char) * 7);
    id.buffer = 7;
    id.length = 7;
    strcpy(id7.str, "length");
    new_node.data_type = DTYPE_INT;
    new_node.can_be_nil = false;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 1;
    new_node.params = malloc(sizeof(func_params));
    new_node.params->name.str = NULL;
    new_node.params->type = DTYPE_STRING;
    bst_insert(&main_stack.array[main_stack.topIndex], id7.str, new_node);
    // func substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
    StringValue id8;
    strInit(&id8);
    id.str = malloc(sizeof(char) * 10);
    id.buffer = 10;
    id.length = 10;
    strcpy(id8.str, "substring");
    new_node.data_type = DTYPE_STRING;
    new_node.can_be_nil = true;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 3;
    new_node.params = malloc(sizeof(func_params));

    new_node.params->name.str = malloc(sizeof(char) * 2);
    new_node.params->name.str = "of";
    new_node.params->type = DTYPE_STRING;
    new_node.params->next = malloc(sizeof(func_params));

    new_node.params->next->name.str = malloc(sizeof(char) * 10);
    new_node.params->next->name.str = "startingAt";
    new_node.params->next->type = DTYPE_INT;
    new_node.params->next->next = malloc(sizeof(func_params));

    new_node.params->next->next->name.str = malloc(sizeof(char) * 12);
    new_node.params->next->next->name.str = "endingBefore";
    new_node.params->next->next->type = DTYPE_INT;

    bst_insert(&main_stack.array[main_stack.topIndex], id8.str, new_node);
    // func ord(_ 𝑐 : String) -> Int
    StringValue id9;
    strInit(&id9);
    id.str = malloc(sizeof(char) * 4);
    id.buffer = 4;
    id.length = 4;
    strcpy(id9.str, "ord");
    new_node.data_type = DTYPE_INT;
    new_node.can_be_nil = false;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 1;

    new_node.params = malloc(sizeof(func_params));
    new_node.params->name.str = NULL;
    new_node.params->type = DTYPE_STRING;

    bst_insert(&main_stack.array[main_stack.topIndex], id9.str, new_node);
    // func chr(_ 𝑖 : Int) -> String
    StringValue id10;
    strInit(&id10);
    id.str = malloc(sizeof(char) * 4);
    id.buffer = 4;
    id.length = 4;
    strcpy(id10.str, "chr");
    new_node.data_type = DTYPE_STRING;
    new_node.can_be_nil = false;
    new_node.symbol_type = TYPE_FNC;
    new_node.param_number = 1;

    new_node.params = malloc(sizeof(func_params));
    new_node.params->name.str = NULL;
    new_node.params->type = DTYPE_INT;

    bst_insert(&main_stack.array[main_stack.topIndex], id10.str, new_node);
}
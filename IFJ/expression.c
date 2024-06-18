/**
 * Project - IFJ Projekt 2023
 *
 * @author Vaclav Zapletal xzaple40
 * @author Oleg Andriichuk xandri07
 * @author Artem Vereninov xveren00
 * @author Denys Dumych xdumyc00
 *
 * @brief Check expression syntax and creat AST tree
 */

#include "expression.h"

prec_symbols prec_table[14][14] = {
    /*   +  *  (  )  i  $  -  /  <  >  <= >= == !=    */
        {R, S, S, R, S, R, R, S, R, R, R, R, R, R},  // +
        {R, R, S, R, S, R, R, R, R, R, R, R, R, R},  // *
        {S, S, S, W, S, N, S, S, S, S, S, S, S, S},  // (
        {R, R, N, R, N, R, R, R, R, R, R, R, R, R},  // )
        {R, R, N, R, N, R, R, R, R, R, R, R, R, R},  // i
        {S, S, S, N, S, N, S, S, S, S, S, S, S, S},  // $
        {R, S, S, R, S, R, R, S, R, R, R, R, R, R},  // -
        {R, R, S, R, S, R, R, R, R, R, R, R, R, R},  // /
        {S, S, S, R, S, R, S, S, N, N, N, N, N, N},  // <
        {S, S, S, R, S, R, S, S, N, N, N, N, N, N},  // >
        {S, S, S, R, S, R, S, S, N, N, N, N, N, N},  // <=
        {S, S, S, R, S, R, S, S, N, N, N, N, N, N},  // >=
        {S, S, S, R, S, R, S, S, N, N, N, N, N, N},  // ==
        {S, S, S, R, S, R, S, S, N, N, N, N, N, N}   // !=
};

void stackInit(Stack_exp *stack) {
    stack->top = NULL;
}

void stackPush(Stack_exp *stack, prec_symbols data) {
    Stack_item *newElem = malloc(sizeof(Stack_item));
    if (newElem == NULL) {
        exit_program(99, "Memory allocation failure");
    }
    newElem->data = data;
    if (stack->top != NULL) {
        newElem->next = stack->top;
    } else {
        newElem->next = NULL;
    }
    stack->top = newElem;
}

void stackShiftPush(Stack_exp *stack) {
    Stack_item *tmp;
    Stack_item *prev;
    tmp = stack->top;
    prev = NULL;

    while (tmp->data != EMPTY) {
        if (tmp->data >= PLUS && tmp->data <= NOT_EQUAL) {
            Stack_item *new_element = malloc(sizeof(struct stack_el));
            if (new_element == NULL) {
                exit_program(99, "Memory allocation failure");
            }
            new_element->data = S;
            new_element->token = NULL;
            new_element->tree = NULL;
            if (prev != NULL) {
                new_element->next = prev->next;
                prev->next = new_element;
                return;
            } else {
                new_element->next = tmp;
                stack->top = new_element;
                return;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

Stack_item *stackPop(Stack_exp *stack) {
    if (stack->top->data != EMPTY) {
        Stack_item *to_return = stack->top;
        Stack_item *next = stack->top->next;
        stack->top = next;
        return to_return;
    } else {
        return NULL;
    }
}

prec_symbols check_token_symbol(Token *token) {
    switch (token->type) {
        case L_LBRACKET:
            return LBRACKET;
        case L_RBRACKET:
            return RBRACKET;
        case L_MULTIPLY:
            return MUL;
        case L_DIVIDE:
            return DIV;
        case L_PLUS:
            return PLUS;
        case L_MINUS:
            return MINUS;
        case L_INT:
        case L_DOUBLE:
        case L_EXP:
        case L_STRING:
        case L_STRING_PLUS:
        case L_ID:
            return ID;
        case L_EQUAL:
            return EQUAL;
        case L_NOT_EQUAL:
            return NOT_EQUAL;
        case L_LESS:
            return LESS;
        case L_GREATER:
            return GREATER;
        case L_LESS_EQUAL:
            return LESS_EQUAL;
        case L_GREATER_EQUAL:
            return GREATER_EQUAL;
        case L_KEYWORD:
        case L_CURL_LBRACKET:
        case L_CURL_RBRACKET:
        case L_EOF:
            return DOLLAR;
        default:
            return INVALID;
    }
}

prec_symbols stack_top_terminal(Stack_exp *stack) {
    Stack_item *tmp;
    tmp = stack->top;
    while (tmp->data != EMPTY) {
        if (tmp->data >= PLUS && tmp->data <= NOT_EQUAL) {
            return tmp->data;
        }
        tmp = tmp->next;
    }
    return EMPTY;
}

int builtCorrect(Stack_exp *stack) {
    if (stack->top != NULL && stack->top->data == E &&
        stack->top->next != NULL &&
        stack->top->next->data == DOLLAR &&
        stack->top->next->next != NULL &&
        stack->top->next->next->data == EMPTY) {
        return 1;
    }
    return 0;
}

int rule_reduction(Stack_exp *stack) {
    Stack_item *stack_data[4];
    for (int j = 0; j < 4; j++) {
        stack_data[j] = malloc(sizeof(Stack_item));
        if (stack_data[j] == NULL) {
            exit_program(99, "Memory allocation failure");
        }
    }

    for (int i = 0; i < 4; i++) {
        stack_data[i] = stackPop(stack);
        if ((stack_data[i])->data == S) {
            break;
        }
    }
    if (stack_data[0]->data == S) {
        return 0;
    }
    // E -> i
    if (stack_data[1]->data == S && stack_data[0]->data == ID) {
        stackPush(stack, E);
        if (stack_data[0]->token->type == L_ID) {
        }
        stack->top->tree = make_leaf(stack_data[0]->token);
        stack->top->token = stack_data[0]->token;
        return 1;
    }
    // E -> (E)
    if (stack_data[0]->data == RBRACKET && stack_data[1]->data == E &&
        stack_data[2]->data == LBRACKET) {
        stackPush(stack, E);
        stack->top->token = stack_data[1]->token;
        stack->top->tree = stack_data[1]->tree;
        return 1;
    }
    // E -> E op E
    if (stack_data[0]->data == E && stack_data[2]->data == E) {
        ast_node_t *tree_ptr = NULL;
        Token *operator = (Token *) malloc(sizeof(Token));
        if (operator == NULL) {
            exit_program(99, "Memory allocation failure");
        }
        switch (stack_data[1]->data) {
            case PLUS:
                operator->type = L_PLUS;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case MUL:
                operator->type = L_MULTIPLY;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case DIV:
                operator->type = L_DIVIDE;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case GREATER:
                operator->type = L_GREATER;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case GREATER_EQUAL:
                operator->type = L_GREATER_EQUAL;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case LESS:
                operator->type = L_LESS;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case LESS_EQUAL:
                operator->type = L_LESS_EQUAL;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case MINUS:
                operator->type = L_MINUS;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case EQUAL:
                operator->type = L_EQUAL;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            case NOT_EQUAL:
                operator->type = L_NOT_EQUAL;
                tree_ptr = make_tree(operator, stack_data[2]->tree, stack_data[0]->tree);
                break;
            default:
                break;
        }
        stackPush(stack, E);
        stack->top->tree = tree_ptr;
        return 1;
    } else {
        return 0;
    }
}

int id_define_check(Token *current_token, node_data *new_node, Stack *main_stack, id_data_type *exp_data_type) {
    if (current_token->type == L_ID) {
        bst_node_t *tmp;
        if ((tmp = element_is_declared(main_stack, current_token->data.stringValue.str)) != NULL) {
            if (*exp_data_type != DTYPE_VOID) {
                if (*exp_data_type != tmp->node_data->data_type) {
                    return 7;
                }
            } else {
                *exp_data_type = tmp->node_data->data_type;
                update_type_dt(new_node, *exp_data_type);
            }
        } else {
            return 5;
        }
    } else {
        if (*exp_data_type != DTYPE_VOID) {
            if ((*exp_data_type == DTYPE_INT && current_token->type != L_INT)
                || (*exp_data_type == DTYPE_DOUBLE && current_token->type != L_DOUBLE)
                || (*exp_data_type == DTYPE_STRING && current_token->type != L_STRING)) {
                return 7;
            }
        } else {
            switch (current_token->type) {
                case L_INT:
                    *exp_data_type = DTYPE_INT;
                    break;
                case L_DOUBLE:
                    *exp_data_type = DTYPE_DOUBLE;
                    break;
                case L_STRING:
                    *exp_data_type = DTYPE_STRING;
                    break;
                default:
                    break;
            }
            update_type_dt(new_node, *exp_data_type);
        }
    }
    return 0;
}

int exp_end_check(Token *current_token, Token *prev_token, node_data *new_node, Stack *main_stack,
                  prec_symbols *current_token_enum, id_data_type *exp_data_type, bool is_expression) {
    if (is_expression) {
        if (!(prev_token->type == L_PLUS || prev_token->type == L_MINUS ||
              prev_token->type == L_MULTIPLY
              || prev_token->type == L_DIVIDE || prev_token->type == L_LBRACKET ||
              prev_token->type == L_LESS
              || prev_token->type == L_LESS_EQUAL || prev_token->type == L_GREATER ||
              prev_token->type == L_GREATER_EQUAL
              || prev_token->type == L_EQUAL || prev_token->type == L_NOT_EQUAL)) {
            *current_token_enum = DOLLAR;
        } else {
            int check_err = id_define_check(current_token, new_node, main_stack, exp_data_type);
            if (check_err != 0) {
                return check_err;
            }
        }
    } else if (current_token->type == L_ID) {
        bst_node_t *tmp;
        if ((tmp = element_is_declared(main_stack, current_token->data.stringValue.str)) == NULL) {
            return 5;
        }
    }
    return 0;
}

int expression(Token *used_token, bool is_expression, node_data *new_node, Stack *main_stack, ast_node_t **tree) {
    Stack_exp stack;
    stackInit(&stack);
    stackPush(&stack, EMPTY);
    Token *prev_token = NULL;
    Token *current_token;
    prec_symbols current_token_enum;
    id_data_type exp_data_type;

    if (is_expression && new_node != NULL) {
        exp_data_type = new_node->data_type;
    }

    if (used_token != NULL) {
        current_token_enum = check_token_symbol(used_token);
        current_token = used_token;
        if (current_token_enum == INVALID) {
            return 2;
        } else if (current_token_enum == ID) {
            if (is_expression) {
                int err = id_define_check(current_token, new_node, main_stack, &exp_data_type);
                if (err != 0) {
                    return err;
                }
            } else if (current_token->type == L_ID) {
                bst_node_t *tmp;
                if ((tmp = element_is_declared(main_stack, current_token->data.stringValue.str)) == NULL) {
                    return 5;
                }
            }
        }

    } else {
        current_token = exp_token();
        current_token_enum = check_token_symbol(current_token);
        if (current_token_enum == INVALID) {
            return 2;
        } else if (current_token_enum == ID) {
            if (is_expression) {
                int err = id_define_check(current_token, new_node, main_stack, &exp_data_type);
                if (err != 0) {
                    return err;
                }
            } else if (current_token->type == L_ID) {
                bst_node_t *tmp;
                if ((tmp = element_is_declared(main_stack, current_token->data.stringValue.str)) == NULL) {
                    return 5;
                }
            }
        }
    }
    prec_symbols top;
    stackPush(&stack, DOLLAR);

    do {
        top = stack_top_terminal(&stack);
        switch (prec_table[top][current_token_enum]) {
            case W:
                prev_token = current_token;
                stackPush(&stack, current_token_enum);
                stack.top->token = current_token;

                current_token = exp_token();

                current_token_enum = check_token_symbol(current_token);
                if (current_token_enum == INVALID) {
                    return 2;
                } else if (current_token_enum == ID) {
                    int err = exp_end_check(current_token, prev_token, new_node, main_stack,
                                            &current_token_enum, &exp_data_type, is_expression);
                    if (err != 0) {
                        return err;
                    }
                }
                break;
            case S:
                stackShiftPush(&stack);
                prev_token = current_token;
                stackPush(&stack, current_token_enum);
                stack.top->token = current_token;
                current_token = exp_token();
                current_token_enum = check_token_symbol(current_token);
                if (current_token_enum == INVALID) {
                    return 2;
                } else if (current_token_enum == ID) {
                    int err = exp_end_check(current_token, prev_token, new_node, main_stack,
                                            &current_token_enum, &exp_data_type, is_expression);
                    if (err != 0) {
                        return err;
                    }
                }
                break;
            case R:
                if (!rule_reduction(&stack)) {
                    return 2;
                }
                break;
            case N:
                return 2;
            default:
                return 2;
        }
    } while ((current_token_enum != DOLLAR) || (builtCorrect(&stack) == 0));
    if (builtCorrect(&stack)) {
        if (!is_expression) {
            if (current_token->type != L_CURL_LBRACKET) {
                return 2;
            }
        }

        *tree = stack.top->tree;
        return 0;
    }
    return 2;
}
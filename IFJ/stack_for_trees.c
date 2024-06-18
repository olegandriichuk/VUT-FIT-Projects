#include "stack_for_trees.h"
#include <stdlib.h>

int STACK_SIZE = MAX_STACK;
bool error_flag;
bool solved;

void Stack_Error(int error_code) {
    static const char *SERR_STRINGS[MAX_SERR + 1] = {
            "Unknown error",
            "Stack error: INIT",
            "Stack error: PUSH",
            "Stack error: TOP"
    };

    if (error_code <= 0 || error_code > MAX_SERR) {
        error_code = 0;
    }

    printf("%s\n", SERR_STRINGS[error_code]);
    error_flag = true;
}

void Stack_Init(Stack *stack) {
    if (stack == NULL) {
        Stack_Error(SERR_INIT);
        return;
    }

    stack->array = malloc(sizeof(bst_node_t));
    if (stack->array == NULL) {
        Stack_Error(SERR_INIT);
    }
    stack->topIndex = -1;
    stack->size = 1;

}

bool Stack_IsEmpty(const Stack *stack) {
    return stack->topIndex == -1;
}

bool Stack_IsFull(const Stack *stack) {
    return stack->topIndex == stack->size - 1;
}

void Stack_Top(const Stack *stack, bst_node_t *dataPtr) {
    if (Stack_IsEmpty(stack)) {
        Stack_Error(SERR_TOP);
    } else {
        *dataPtr = *stack->array[stack->topIndex];//*(stack->array+sizeof(bst_node_t)*stack->topIndex);
    }
}


void Stack_Pop(Stack *stack) {
    if (!Stack_IsEmpty(stack)) {
        bst_dispose(&stack->array[stack->topIndex]);
        stack->topIndex -= 1;
    }
}

void Stack_Push(Stack *stack) {
    bst_node_t new_tree;
    bst_node_t *ptr_to_tree = &new_tree;
    bst_init(&ptr_to_tree);
    if (Stack_IsFull(stack))
        stack->array = realloc(stack->array, (2 * stack->size) * sizeof(bst_node_t));
    if (stack->array) {
        stack->topIndex += 1;
        stack->array[stack->topIndex] = ptr_to_tree;
        stack->size *= 2;
    } else {
        stack->size = 0;
        stack->topIndex = -1;
        fprintf(stderr, "STACK MEMORY REALLOCATION FAILED");
        exit(99);
    }
}

void Stack_Dispose(Stack *stack) {
    stack->size = 0;
    stack->topIndex = -1;
    free(stack->array);
    stack->array = NULL;
    free(stack);
}

bst_node_t *element_is_declared_in_local_scope(Stack *stack, char *key) {
    //printf("%s\n", token->data.stringValue.str);
    return bst_search(stack->array[stack->topIndex], key);
//    if (bst_search(stack->array[stack->topIndex], key))
//        return true;
//    return false;
}


bst_node_t *element_is_declared(Stack *stack, char *key) {
    for (int i = stack->topIndex; i >= 0; i--) {
        if (bst_search(stack->array[i], key))
            //return true;
            return bst_search(stack->array[i], key);
    }
    return NULL;
}

void update_type(node_data *new_node, Keywords keyword) {
        switch (keyword) {
            case K_INT:
                new_node->data_type = DTYPE_INT;
                break;
            case K_DOUBLE:
                new_node->data_type = DTYPE_DOUBLE;
                break;
            case K_STRING:
                new_node->data_type = DTYPE_STRING;
                break;
            default:
                break;
        }
}

void update_type_t(node_data *new_node, Types type) {
    switch (type) {
        case L_INT:
            new_node->data_type = DTYPE_INT;
            break;
        case L_DOUBLE:
            new_node->data_type = DTYPE_DOUBLE;
            break;
        case L_STRING:
            new_node->data_type = DTYPE_STRING;
            break;
        default:
            break;
    }
}

void update_type_dt(node_data *new_node, id_data_type data_type) {
    switch (data_type) {
        case DTYPE_INT:
            new_node->data_type = DTYPE_INT;
            break;
        case DTYPE_DOUBLE:
            new_node->data_type = DTYPE_DOUBLE;
            break;
        case DTYPE_STRING:
            new_node->data_type = DTYPE_STRING;
            break;
        default:
            break;
    }
}

void update_type_params(func_params *params, Keywords keyword) {
    switch (keyword) {
        case K_INT:
            params->type = DTYPE_INT;
            break;
        case K_DOUBLE:
            params->type = DTYPE_DOUBLE;
            break;
        case K_STRING:
            params->type = DTYPE_STRING;
            break;
        default:
            break;
    }
}

bool not_compatible (bst_node_t* node, Types type){
    return ((type == L_INT && node->node_data->data_type != DTYPE_INT) || (type == L_DOUBLE && node->node_data->data_type != DTYPE_DOUBLE) || (type == L_STRING && node->node_data->data_type != DTYPE_STRING));
}

bool not_compatible_nd(node_data* node, Types type){
    return ((type == L_INT && node->data_type != DTYPE_INT) || (type == L_DOUBLE && node->data_type != DTYPE_DOUBLE) || (type == L_STRING && node->data_type != DTYPE_STRING));
}

bool not_compatible_param(func_params *param, Types type) {
    return ((type == L_INT && param->type != DTYPE_INT) || (type == L_DOUBLE && param->type != DTYPE_DOUBLE) ||
            (type == L_STRING && param->type != DTYPE_STRING));
}
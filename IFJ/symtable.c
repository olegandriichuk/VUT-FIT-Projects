#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"


void bst_add_node_to_items(bst_node_t* node, bst_items_t *items) {
    if (items->capacity < items->size + 1)
    {
        items->capacity = items->capacity * 2 + 8;
        items->nodes = realloc(items->nodes, items->capacity * (sizeof(bst_node_t*)));
    }
    items->nodes[items->size] = node;
    items->size++;
}


void bst_init(bst_node_t **tree) {
    (*tree) = NULL;
}


bst_node_t* bst_search(bst_node_t *tree, char* key) {
    if(tree == NULL){
        return false;
    }else {
        if (strcmp(tree->key, key) == 0) {
            return tree;
        } else if (strcmp(key, tree->key) > 0) {
            return bst_search(tree->right, key);
        } else {
            return bst_search(tree->left, key);
        }
    }
}

void bst_insert(bst_node_t **tree, char* key, node_data value) {
    if((*tree) == NULL){
        bst_node_t* newNodePtr = (bst_node_t*) malloc(sizeof(bst_node_t));
        if(newNodePtr == NULL){
            return;
        }
        newNodePtr->node_data = (node_data*)malloc(sizeof(node_data));
        newNodePtr->key = key;
        newNodePtr->node_data->data_type = value.data_type;
        newNodePtr->node_data->symbol_type = value.symbol_type;
        newNodePtr->node_data->param_number = value.param_number;
        newNodePtr->node_data->can_be_nil = value.can_be_nil;
        newNodePtr->node_data->params = value.params;
        newNodePtr->left = NULL;
        newNodePtr->right = NULL;
        (*tree) = newNodePtr;
    }else if (strcmp(key,  (*tree)->key) < 0){
        bst_insert(&(*tree)->left, key, value);
    }else if(strcmp(key, (*tree)->key) > 0){
        bst_insert(&(*tree)->right, key, value);
    }else{
        (*tree)->node_data = &value;
    }
    bst_balance(tree);
}

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if((*tree)->right != NULL){
        bst_replace_by_rightmost(target,&(*tree)->right);
    }else{
        target->key = (*tree)->key;
        target->node_data = (*tree)->node_data;
        free((*tree));
        (*tree) = NULL;
    }
}


void bst_delete(bst_node_t **tree, char* key) {
    if((*tree) == NULL){
        return;
    } else{
        if(strcmp(key,(*tree)->key) > 0 ){
            bst_delete(&(*tree)->right, key);
        } else if(strcmp(key,(*tree)->key) < 0 ){
            bst_delete(&(*tree)->left, key);
        } else{
            if((*tree)->left == NULL){
                bst_node_t* temp = (*tree);
                (*tree) = (*tree)->right;
                free(temp);
            } else if((*tree)->right == NULL){
                bst_node_t* temp = (*tree);
                (*tree) = (*tree)->left;
                free(temp);
            } else if((*tree)->left == NULL && (*tree)->right == NULL){
                free(*tree);
                (*tree) = NULL;
            }
            else{
                bst_replace_by_rightmost((*tree), &(*tree)->left);
            }
        }
    }
    bst_balance(tree);

}


void bst_dispose(bst_node_t **tree) {
    if((*tree) == NULL){
        return;
    } else{
        bst_dispose(&(*tree)->left);
        bst_dispose(&(*tree)->right);
        if( !(*tree)->key ){
            free((*tree)->key);
        }
        if(!(*tree)->node_data){
            free((*tree)->node_data);
        }
        if(!(*tree)){
            free(*tree);
        }
        (*tree) = NULL;

    }
}

void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if(tree != NULL){
        bst_inorder(tree->left, items);
        bst_add_node_to_items(tree,items);
        bst_inorder(tree->right, items);
    }

}

bst_node_t *NewNode(bst_items_t *all_nodes, int mid) {
    bst_node_t *root = (bst_node_t *) malloc(sizeof(bst_node_t));
    root->node_data = all_nodes->nodes[mid]->node_data;
    root->key = all_nodes->nodes[mid]->key;
    root->left = NULL;
    root->right = NULL;
    return root;
}

bst_node_t *arrayToBst(bst_items_t *all_nodes, int start, int end) {
    if (start > end)
        return NULL;

    //Recursive approach to balancing the tree
    int mid = (start + end) / 2;
    bst_node_t *root = NewNode(all_nodes, mid);
    root->left = arrayToBst(all_nodes, start, mid - 1);
    root->right = arrayToBst(all_nodes, mid + 1, end);

    return root;
}

void bst_balance(bst_node_t **tree) {
    //Initializing of the array
    bst_items_t *all_nodes = malloc(sizeof(bst_items_t));
    all_nodes->capacity = 0;
    all_nodes->nodes = NULL;
    all_nodes->size = 0;

    //Inorder array
    bst_inorder(*tree, all_nodes);

    //Creating balanced tree from sorted array
    *tree = arrayToBst(all_nodes, 0, all_nodes->size - 1);

    //Clean
    for (int i = 0; i < all_nodes->size; ++i) {
        free(all_nodes->nodes[i]);
    }
    free(all_nodes->nodes);
    free(all_nodes);
}

int max (int n1, int n2)
{ // funkce vrátí hodnotu většího ze dvou parametrů
    if (n1 > n2) {
        return n1;
    } else {
        return n2;
    }
}

int bst_height(bst_node_t *tree) {
    if (tree != NULL) {
        return max(bst_height(tree->left), bst_height(tree->right)) + 1;
    } else {
        return 0;
    }
}

void add_new_element(func_params **params, func_params source) {
    if ((*params) == NULL) {
        *params = malloc(sizeof(func_params));
        strInit(&(*params)->name);
        (*params)->name = source.name;
        (*params)->type = source.type;
        (*params)->next = malloc(sizeof(func_params));
        
    } else {
        func_params *tmp = *params;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        strInit(&tmp->name);
        tmp->name = source.name;
        tmp->type = source.type;
        tmp->next = malloc(sizeof(func_params));
    }
}
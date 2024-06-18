#ifndef IAL_BTREE_H
#define IAL_BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "str.h"
#include "scanner.h"
struct n_data;
struct bst_node;
typedef enum {
    TYPE_LET ,
    TYPE_FNC ,
    TYPE_VAR,
} id_type;

typedef enum {

    DTYPE_INT,
    DTYPE_DOUBLE,
    DTYPE_STRING,
    DTYPE_VOID, // if function does not return anything
    DTYPE_NULL
} id_data_type;

typedef struct linked_list {
    StringValue name;
    id_data_type type;
    struct linked_list* next;
} func_params;

// Uzol stromu
typedef struct bst_node {
    char* key;             // klic
    struct n_data *node_data;              // hodnota
    struct bst_node *left;  // levy potomek
    struct bst_node *right; // pravy potomek
} bst_node_t;

typedef struct n_data {
    id_type symbol_type;
    id_data_type data_type;
    int param_number; // number of parameters in function
    bool can_be_nil;
    func_params *params;
} node_data;
// Pole uzlu
typedef struct bst_items {
    bst_node_t **nodes;     // pole uzlu
    int capacity;           // kapacita alokované paměti v počtu položek
    int size;               // aktuální velikost pole v počtu položek
} bst_items_t;

/*
 * Add a node to a collection of items.
 *
 * Adds the given 'node' to the 'items' collection. If the collection's
 * capacity is insufficient, it is resized dynamically.
 *
 * Parameters:
 *   - node: Node to be added to the collection.
 *   - items: Collection of items to which the node is added.
 */
void bst_add_node_to_items(bst_node_t* node, bst_items_t *items);
/*
 * Tree initialization.
 *
 * The user must ensure that initialization is not repeatedly called on an
 * already initialized tree. Otherwise, it may lead to memory leaks. Since
 * an uninitialized pointer has an undefined value, it is not possible to
 * detect this within the function.
 */
void bst_init(bst_node_t **tree);

/*
 * Search for a node in the tree.
 *
 * If successful, the function returns true and stores the value of the node
 * in the variable 'value'. Otherwise, the function returns false, and the
 * 'value' variable remains unchanged.
 *
 */
bst_node_t* bst_search(bst_node_t *tree, char* key);

/*
 * Insert a node into the tree.
 *
 * If a node with the specified key already exists in the tree, replace its
 * value. Otherwise, insert a new leaf node.
 *
 * The resulting tree must satisfy the condition of a binary search tree —
 * the left subtree of a node contains only smaller keys, and the right subtree
 * contains only larger keys.
 *
 * Implement the function recursively without using additional helper functions.
 */
void bst_insert(bst_node_t **tree, char* key, node_data value);

/*
 * Helper function that replaces a node with its rightmost descendant.
 *
 * The key and value of the 'target' node will be replaced with the key and
 * value of the rightmost node in the subtree 'tree'. The rightmost descendant
 * will be removed. The function correctly frees all allocated resources of
 * the removed node.
 *
 * The function assumes that the value of 'tree' is not NULL.
 *
 * This helper function will be used in the implementation of the 'bst_delete'
 * function.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);

/*
 * Remove a node from the tree.
 *
 * If a node with the specified key does not exist, the function does nothing.
 * If the removed node has only one subtree, its parent inherits the subtree.
 * If the removed node has both subtrees, it is replaced by the rightmost node
 * of the left subtree. The rightmost node may not be a leaf.
 */
void bst_delete(bst_node_t **tree, char* key);

/*
 * Destroy the entire tree.
 *
 * After destruction, the entire tree will be in the same state as after
 * initialization. The function correctly frees all allocated resources of
 * the destroyed nodes.
 */
void bst_dispose(bst_node_t **tree);

/*
 * Inorder traversal of a BST, adding nodes to a collection.
 *
 * Recursively traverses the BST in ascending order and adds each node to the
 * specified 'items' collection.
 *
 * Parameters:
 *   - tree: Root node of the BST.
 *   - items: Collection to which nodes are added.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items);
/**
 *
 * @param tree
 * @return height of binary search tree
 */
int bst_height(bst_node_t *tree);

/**
 * @param n1 first number
 * @param n2 second number
 * @return maximum of two numbers
 */
int max (int n1, int n2);

/*
 * Convert an array of nodes to a balanced BST using recursion.
 *
 *
 *   @param all_nodes Collection of nodes for constructing the BST.
 *   @start Start index of the subarray.
 *   @end End index of the subarray.
 *
 * Returns:
 *   Root of the constructed balanced BST.

 */
bst_node_t *arrayToBst(bst_items_t *all_nodes, int start, int end);
/**
 * insert new elements to linked list
 * @param params pointer to the first element of linked list
 * @param source temporary structure for saving data before last update
 */
void add_new_element(func_params **params, func_params source);

/**brief
 * balance current binary tree search
 * @param root
 */
void bst_balance(bst_node_t** root);

#endif

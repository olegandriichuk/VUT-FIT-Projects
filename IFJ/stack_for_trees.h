#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"

/**
 * Hodnota MAX_STACK udává skutečnou velikost dynamického pole pro uložení
 * hodnot zásobníku.
 * Při implementaci hodnotu MAX_STACK vůbec nepoužívejte.
 */
#define MAX_STACK 20

/**
 * Při implementaci operací nad ADT zásobník předpokládejte, že velikost tohoto
 * pole je pouze STACK_SIZE.
 * Umožní to jednoduše měnit velikot zásobníku v průběhu testování.
 */
extern int STACK_SIZE;

/** Globální proměnná - indikuje, zda operace volala chybu. */
extern bool error_flag;
/** Globální proměnná - indikuje, zda byla operace řešena. */
extern bool solved;

/** Celkový počet možných chyb. */
#define MAX_SERR    3
/** Chyba při Stack_Init. */
#define SERR_INIT   1
/** Chyba při Stack_Push. */
#define SERR_PUSH   2
/** Chyba při Stack_Top. */
#define SERR_TOP    3

/** ADT zásobník implementovaný v dynamickém poli. */
typedef struct {
	/** Pole pro uložení hodnot. */
	bst_node_t **array;
	/** Index prvku na vrcholu zásobníku. */
	int topIndex;
    int size;
} Stack;

void Stack_Error( int );

void Stack_Init( Stack * );

bool Stack_IsEmpty( const Stack * );

bool Stack_IsFull( const Stack * );

void Stack_Top( const Stack *, bst_node_t * );

void Stack_Pop( Stack * );

void Stack_Push( Stack *);

void Stack_Dispose( Stack * );

void update_type(node_data *new_node, Keywords keyword);
void update_type_t(node_data *new_node, Types type);
void update_type_dt(node_data *new_node, id_data_type data_type);
void update_type_params(func_params *params, Keywords keyword);


bst_node_t *element_is_declared(Stack *stack, char *key);

bst_node_t *element_is_declared_in_local_scope(Stack *stack, char *key);

bool not_compatible(bst_node_t* node, Types type);
bool not_compatible_nd(node_data * node, Types type);
bool not_compatible_param(func_params *param, Types type);

#endif

/* Konec hlavičkového souboru c202.h */
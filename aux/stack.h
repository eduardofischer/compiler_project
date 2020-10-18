#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "hashtable.h"

typedef struct stack_item {
  HT_ENTRY **table;
  struct stack_item *next;
} STACK_ITEM;

// Cria uma nova tabela de símbolos no topo da pilha
STACK_ITEM *new_scope(STACK_ITEM *stack);

// Retorna a tabela de símbolos no topo da pilha
HT_ENTRY **top(STACK_ITEM *stack);

// Retorna a tabela de símbolos na posição pos da pilha
HT_ENTRY **peek(STACK_ITEM *stack, int pos);

// Remove e libera a tabela de símbolos no topo da pilha
STACK_ITEM *pop(STACK_ITEM *stack);

// Procura o símbolo em todos os escopos
HT_ENTRY *search_all_scopes(STACK_ITEM *stack, char *key);

// Procura o símbolo em todos os escopos
HT_ENTRY *search_local_scope(STACK_ITEM *stack, char *key);

#endif

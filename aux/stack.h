#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "hashtable.h"

// Inicializa uma pilha de escopos
STACK_ITEM *init_scope_stack();

// Cria uma nova tabela de símbolos no topo da pilha
STACK_ITEM *new_scope(STACK_ITEM *stack);

// Retorna a tabela de símbolos no topo da pilha
SYMBOL_ENTRY **top(STACK_ITEM *stack);

// Retorna a tabela de símbolos na posição pos da pilha
SYMBOL_ENTRY **peek(STACK_ITEM *stack, int pos);

// Remove e libera a tabela de símbolos no topo da pilha
STACK_ITEM *destroy_scope(STACK_ITEM *stack);

// Procura o símbolo em todos os escopos
SYMBOL_ENTRY *search_all_scopes(STACK_ITEM *stack, char *key);

// Procura o símbolo em todos os escopos
SYMBOL_ENTRY *search_local_scope(STACK_ITEM *stack, char *key);

// Injeta os argumentos da função no seu escopo
void inject_arguments(STACK_ITEM *stack, ARG_LIST *args);

// Verifica se o escopo é global
int is_global_scope(STACK_ITEM *stack);

#endif

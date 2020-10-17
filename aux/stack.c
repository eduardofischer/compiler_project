#include "stack.h"

// Cria uma nova tabela de símbolos no topo da pilha
STACK_ITEM *new_scope(STACK_ITEM *stack) {
  STACK_ITEM *item = malloc(sizeof(STACK_ITEM));
  item->table = malloc(sizeof(HT_ENTRY) * HT_SIZE);
  item->next = stack;
  
  return item;
}

// Retorna a tabela de símbolos no topo da pilha
HT_ENTRY **top(STACK_ITEM *stack) {
  if (stack == NULL)
    return NULL;

  return stack->table;
}

// TODO: Retorna a tabela de símbolos na posição pos da pilha
HT_ENTRY **peek(STACK_ITEM *stack, int pos) {
  return NULL;
}

// Remove e libera a tabela de símbolos no topo da pilha
STACK_ITEM *pop(STACK_ITEM *stack) {
  if (stack == NULL)
    return NULL; // Pilha não existente

  free_ht(stack->table);

  return stack->next;
}
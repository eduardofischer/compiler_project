#include "stack.h"

// Cria uma nova tabela de símbolos no topo da pilha
STACK_ITEM *new_scope(STACK_ITEM *stack) {
  STACK_ITEM *item = malloc(sizeof(STACK_ITEM));
  item->table = malloc(sizeof(HT_ENTRY) * HT_SIZE);
  memset(item->table, 0, sizeof(HT_ENTRY) * HT_SIZE);
  item->next = stack;
  
  return item;
}

// Retorna a tabela de símbolos no topo da pilha
HT_ENTRY **top(STACK_ITEM *stack) {
  if (stack == NULL)
    return NULL;

  return stack->table;
}

// Retorna a tabela de símbolos na posição pos da pilha
HT_ENTRY **peek(STACK_ITEM *stack, int pos) {
  if (stack == NULL)
    return NULL;

  STACK_ITEM *item = stack;
  
  for (int i=0; i < pos; i++) {
    if (item->next == NULL)
      return NULL; // Item inexistente na posição pos
    
    item = item->next;
  }

  return item->table;
}

// Remove e libera a tabela de símbolos no topo da pilha
STACK_ITEM *pop(STACK_ITEM *stack) {
  if (stack == NULL)
    return NULL; // Pilha não existente

  free_ht(stack->table);

  return stack->next;
}

// Procura o símbolo em todos os escopos
HT_ENTRY *search_all_scopes(STACK_ITEM *stack, char *key) {
  STACK_ITEM *scope = stack;
  HT_ENTRY *entry;
  while (scope != NULL) {
    if ((entry = get_ht_entry(top(scope), key)) != NULL)
      return entry;
    scope = scope->next;
  };
  return NULL;
}

// Procura o símbolo em todos os escopos
HT_ENTRY *search_local_scope(STACK_ITEM *stack, char *key) {
  HT_ENTRY *entry;
  // Procura o símbolo apenas no escopo atual
  if ((entry = get_ht_entry(top(stack), key)) != NULL)
    return entry;

  return NULL;
}
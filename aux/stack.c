#include "stack.h"

// Inicializa uma pilha de escopos
STACK_ITEM *init_scope_stack() {
  STACK_ITEM *item = malloc(sizeof(STACK_ITEM));
  item->table = malloc(sizeof(SYMBOL_ENTRY) * HT_SIZE);
  memset(item->table, 0, sizeof(SYMBOL_ENTRY) * HT_SIZE);
  item->offset = 0;
  item->next = NULL;
  
  return item;
}

// Cria uma nova tabela de símbolos no topo da pilha
STACK_ITEM *new_scope(STACK_ITEM *stack) {
  STACK_ITEM *item = malloc(sizeof(STACK_ITEM));
  item->table = malloc(sizeof(SYMBOL_ENTRY) * HT_SIZE);
  memset(item->table, 0, sizeof(SYMBOL_ENTRY) * HT_SIZE);
  item->offset = stack->offset;
  item->prev_offset = stack->offset;
  item->next = stack;
  
  return item;
}

// Retorna a tabela de símbolos no topo da pilha
SYMBOL_ENTRY **top(STACK_ITEM *stack) {
  if (stack == NULL)
    return NULL;

  return stack->table;
}

// Retorna a tabela de símbolos na posição pos da pilha
SYMBOL_ENTRY **peek(STACK_ITEM *stack, int pos) {
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
STACK_ITEM *destroy_scope(STACK_ITEM *stack) {
  STACK_ITEM * next;

  if (stack == NULL)
    return NULL; // Pilha não existente

  free_ht(stack->table);
  next = stack->next;
  if (next != NULL)
    next->offset = stack->prev_offset;
  free(stack);
  return next;
}

// Procura o símbolo em todos os escopos
SYMBOL_ENTRY *search_all_scopes(STACK_ITEM *stack, char *key) {
  STACK_ITEM *scope = stack;
  SYMBOL_ENTRY *entry;
  while (scope != NULL) {
    if ((entry = get_ht_entry(top(scope), key)) != NULL) {
      entry->global = is_global_scope(scope);
      return entry;
    }
    scope = scope->next;
  };
  return NULL;
}

// Procura o símbolo em todos os escopos
SYMBOL_ENTRY *search_local_scope(STACK_ITEM *stack, char *key) {
  SYMBOL_ENTRY *entry;
  // Procura o símbolo apenas no escopo atual
  if ((entry = get_ht_entry(top(stack), key)) != NULL)
    return entry;

  return NULL;
}

// Injeta os argumentos da função no seu escopo
void inject_arguments(STACK_ITEM *stack, ARG_LIST *args) {
  ARG_LIST *current = args;
  SYMBOL_ENTRY entry;
  while (current != NULL) {
    entry.key = strdup(current->id);
    entry.data_type = current->type;
    entry.entry_type = ET_VARIABLE;
    entry.size = assign_size(current->type);
    insert_ht_entry(stack, entry);
    current = current->next;
  }
}

// Verifica se o escopo é global
int is_global_scope(STACK_ITEM *stack) {
  return stack->next == NULL;
}
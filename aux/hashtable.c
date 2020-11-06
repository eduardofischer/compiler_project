#include "hashtable.h"

// Calcula a posição de inserção na tabela com base na chave (Utiliza djb2)
int hash(char *key) {
  unsigned int value = 5381;

  int i;
  for(i=0; i < strlen(key); i++)
      value = ((value << 5) + value) + (int)key[i];

  return value % HT_SIZE;
}

// Recupera a entrada na hash table
SYMBOL_ENTRY *get_ht_entry(SYMBOL_ENTRY **table, char *key) {
  int index = hash(key);
  SYMBOL_ENTRY *entry = table[index];

  if(entry == NULL)
    return NULL;

  if(strcmp(entry->key, key) == 0)
    return entry;

  while(entry->next != NULL) {
    entry = entry->next;
    if(strcmp(entry->key, key) == 0)
      return entry;    
  };
}

// Insere uma entrada na hash table
int insert_ht_entry(STACK_ITEM *scope, SYMBOL_ENTRY value) {
  SYMBOL_ENTRY **table = top(scope);

  if(get_ht_entry(table, value.key) != NULL) {
    // free_entry(value);
    return -1; // Entrada já existe na tabela
  }
  int index = hash(value.key);

  SYMBOL_ENTRY *new_entry = malloc(sizeof(SYMBOL_ENTRY));
  *new_entry = value;
  new_entry->key = value.key;
  new_entry->next = NULL;

  // Atualiza deslocamentos de escopo
  if (new_entry->size > 0) {
    new_entry->offset = scope->offset;
    scope->offset += new_entry->size;
    printf("DEBUG: inserindo entrada %s no endereço %d\n", new_entry->key, new_entry->offset);
  }
  
  if(table[index] != NULL)
      new_entry->next = table[index];

  table[index] = new_entry;

  return index;
}

// Libera a memória de uma lista de argumentos
int _free_arg_list(ARG_LIST *list) {
  ARG_LIST *current, *next;
  current = list;

  if (current == NULL)
    return -1; // Lista inexistente

  do {
    next = current->next;
    free(current->id);
    free(current);
    current = next;
  } while (next != NULL);

  return 0;
}

int free_entry(SYMBOL_ENTRY entry) {
  free(entry.key);
  // _free_arg_list(entry.arguments);
}

// Libera a memória da hash table
int free_ht(SYMBOL_ENTRY **table) {
  SYMBOL_ENTRY *current, *next;
  for (int i=0; i < HT_SIZE; i++) {
    current = table[i];
    if (current != NULL) {
      do {
        next = current->next;
        free(current->key);
        // _free_arg_list(current->arguments);
        free(current);
        current = next;
      } while (next != NULL);
    }
  }
  free(table);

  return 0;
}

// Inicializa uma entrada da tabela
SYMBOL_ENTRY init_table_entry(LEX_VALUE valor_lexico, char *key, int entry_type, int data_type){
	SYMBOL_ENTRY table_entry;
  table_entry.key = strdup(key);
	table_entry.line = valor_lexico.line_number;
	table_entry.column = valor_lexico.col_number;
  table_entry.arguments = NULL;
	table_entry.entry_type = entry_type;
	table_entry.data_type = data_type;
	
	return table_entry;
}

int assign_size(int data_type){
  switch (data_type)
  {
  case DT_INT:
    return 4;
  case DT_FLOAT:
    return 8;
  case DT_BOOL:
    return 1;
  case DT_CHAR:
    return 1;
  case DT_STRING:
    return 0;
  default:
    return NOT_DEFINED;
  }
}

int assign_size_var_init(int data_type, TOKEN_VAL valor_lexico){
  switch (data_type)
  {
  case DT_INT:
    return 4;
  case DT_FLOAT:
    return 8;
  case DT_BOOL:
    return 1;
  case DT_CHAR:
    return 1;
  case DT_STRING:
    return 1*strlen(valor_lexico.s);
  default:
    return NOT_DEFINED;
  }
}

int assign_size_vector(int data_type, TOKEN_VAL expression){
  switch (data_type)
  {
  case DT_INT:
    return 4*expression.i;
  case DT_FLOAT:
    return 8*expression.i;
  case DT_BOOL:
    return 1*expression.i;
  case DT_CHAR:
    return 1*expression.i;
  case DT_STRING:
    return 1*expression.i;
  default:
    return NOT_DEFINED;
  }
}
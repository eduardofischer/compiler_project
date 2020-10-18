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
int insert_ht_entry(SYMBOL_ENTRY **table, char *key, SYMBOL_ENTRY value) {
  if(get_ht_entry(table, key) != NULL)
    return -1; // Entrada já existe na tabela
  
  int index = hash(key);

  SYMBOL_ENTRY *new_entry = malloc(sizeof(SYMBOL_ENTRY));
  *new_entry = value;
  new_entry->key = malloc(strlen(key) + 1);
  strcpy(new_entry->key, key);
  new_entry->next = NULL;

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
    free(current);
    current = next;
  } while (next != NULL);

  return 0;
}

// Libera a memória da hash table
int free_ht(SYMBOL_ENTRY **table) {
  SYMBOL_ENTRY *current, *next;
  for (int i=0; i < HT_SIZE; i++) {
    current = table[i];
    if (current != NULL) {
      do {
        next = current->next;
        if (current->key != NULL)
          free(current->key);
        _free_arg_list(current->arguments);
        free(current);
        current = next;
      } while (next != NULL);
    }
  }
  free(table);

  return 0;
}

// Inicializa uma entrada da tabela
SYMBOL_ENTRY init_table_entry(LEX_VALUE valor_lexico, int entry_type, int data_type){
	SYMBOL_ENTRY table_entry;
	table_entry.line = valor_lexico.line_number;
	table_entry.column = valor_lexico.col_number;
  	table_entry.arguments = NULL;
	table_entry.entry_type = entry_type;
	table_entry.data_type = data_type;
	
	return table_entry;
}

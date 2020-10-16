#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include "types.h"

// Natureza do símbolo
#define N_LITERAL 0
#define N_VARIABLE 1
#define N_FUNCTION 2

// Lista de argumentos
typedef struct arg_entry {
  int id;
  int type;
  ARG_ENTRY *next;
} ARG_ENTRY;

// Entrada da tabela de símbolos
typedef struct symbol_entry {
  int line, column;
  int nature; // literal, variável, função, etc...
  int literal_type;
  int size;
  ARG_ENTRY *arguments;
  TOKEN_VAL value;
} SYMBOL_ENTRY;

// Lista de entradas da hash table
typedef struct ht_entry {
  char *key;
  SYMBOL_ENTRY value;
  HT_ENTRY *next;
} HT_ENTRY;

// Calcula a posição de inserção na tabela com base na chave
int hash(char *key, int table_size);

// Recupera a entrada na hash table
HT_ENTRY *get_ht_entry(HT_ENTRY *table, char *key);

// Insere uma entrada na hash table
int insert_ht_entry(HT_ENTRY *table, HT_ENTRY *entry);

// Atualiza uma entrada na hash table
int update_ht_entry(HT_ENTRY *table, HT_ENTRY *entry);

// Remove uma entrada da hash table
int remove_ht_entry(HT_ENTRY *table, HT_ENTRY *entry);

#endif

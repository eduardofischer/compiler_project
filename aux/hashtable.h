#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define HT_SIZE 20

// Calcula a posição de inserção na tabela com base na chave
int hash(char *key);

// Recupera a entrada na hash table
HT_ENTRY *get_ht_entry(HT_ENTRY **table, char *key);

// Insere uma entrada na hash table
int insert_ht_entry(HT_ENTRY **table, char *key, SYMBOL_ENTRY value);

// TODO: Atualiza uma entrada na hash table 
int update_ht_entry(HT_ENTRY **table, HT_ENTRY entry);

// TODO: Remove uma entrada da hash table
int remove_ht_entry(HT_ENTRY **table, char *key);

// Libera a memória referente à hash table
int free_ht(HT_ENTRY **table);

#endif

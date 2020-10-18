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
SYMBOL_ENTRY *get_ht_entry(SYMBOL_ENTRY **table, char *key);

// Insere uma entrada na hash table
int insert_ht_entry(SYMBOL_ENTRY **table, SYMBOL_ENTRY value);

// TODO: Atualiza uma entrada na hash table 
int update_ht_entry(SYMBOL_ENTRY **table, SYMBOL_ENTRY entry);

// TODO: Remove uma entrada da hash table
int remove_ht_entry(SYMBOL_ENTRY **table, char *key);

// Libera a memória referente à hash table
int free_ht(SYMBOL_ENTRY **table);

// Inicializa uma entrada da tabela
SYMBOL_ENTRY init_table_entry(LEX_VALUE valor_lexico, char *key, int entry_type, int data_type);

// Define o tamanho das variáveis
int assign_size(int data_type);
int assign_size_var_init(int data_type, TOKEN_VAL valor_lexico);
int assign_size_vector(int data_type, TOKEN_VAL expression);

#endif

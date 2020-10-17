#ifndef TYPES_H
#define TYPES_H

#define TOKEN_TYPE_SPECIAL 0
#define TOKEN_TYPE_COMPOUND_OP 1
#define TOKEN_TYPE_ID 2
#define TOKEN_TYPE_LIT 3

#define LIT_TYPE_INT 0
#define LIT_TYPE_FLOAT 1
#define LIT_TYPE_BOOL 2
#define LIT_TYPE_CHAR 3
#define LIT_TYPE_STRING 4

#define DT_INT 0
#define DT_FLOAT 1
#define DT_BOOL 2
#define DT_CHAR 3
#define DT_STRING 4

// Enumeração de entry_type (entrada tabela de símbolos)
#define LITERAL 0
#define VARIABLE 1
#define FUNCTION 2

#define NOT_DEFINED 99

typedef union {
  int i;
  float f;
  int b;
  char c;
  char *s;
} TOKEN_VAL;

typedef struct lex_value {
  int line_number, col_number;
  int token_type;
  int literal_type;
  TOKEN_VAL value;
} LEX_VALUE;

// Lista de argumentos
typedef struct arg_entry {
  int id;
  int type;
  struct arg_entry *next;
} ARG_ENTRY;

// Entrada da tabela de símbolos
typedef struct symbol_entry {
  int line, column;
  int entry_type; // literal, variável, função, etc...
  int data_type; // int, float, bool, etc...
  int size;
  ARG_ENTRY *arguments;
  TOKEN_VAL value;
} SYMBOL_ENTRY;

// Lista de entradas da hash table
typedef struct ht_entry {
  char *key;
  SYMBOL_ENTRY value;
  struct ht_entry *next;
} HT_ENTRY;

// Nó da AST
typedef struct ast_node {
  char *label;
  LEX_VALUE *valor_lexico;
  int n_children;
  struct ast_node **children;
} AST_NODE;

typedef struct prod_value {
  AST_NODE *ast_node;
  SYMBOL_ENTRY table_entry;
} PROD_VALUE;

#endif

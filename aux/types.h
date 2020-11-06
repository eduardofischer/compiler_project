#ifndef TYPES_H
#define TYPES_H

#define TOKEN_TYPE_SPECIAL 0
#define TOKEN_TYPE_COMPOUND_OP 1
#define TOKEN_TYPE_ID 2
#define TOKEN_TYPE_LIT 3

#define DT_INT 100
#define DT_FLOAT 101
#define DT_BOOL 102
#define DT_CHAR 103
#define DT_STRING 104

// Enumeração de entry_type (entrada tabela de símbolos)
#define ET_LITERAL 200
#define ET_VARIABLE 201
#define ET_VECTOR 202
#define ET_FUNCTION 203
#define ET_RETURN 204

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
  int data_type;
  TOKEN_VAL value;
} LEX_VALUE;

// Lista de argumentos
typedef struct arg_list {
  char *id;
  int type;
  struct arg_list *next;
} ARG_LIST;

// Entrada da tabela de símbolos
typedef struct symbol_entry {
  char *key;
  int line, column;
  int entry_type; // literal, variável, função, etc...
  int data_type; // int, float, bool, etc...
  int size;
  int offset; // Deslocamento do símbolo dentro do escopo
  ARG_LIST *arguments;
  TOKEN_VAL value;
  struct symbol_entry *next;
} SYMBOL_ENTRY;

typedef struct stack_item {
  SYMBOL_ENTRY **table;
  int offset;
  struct stack_item *next;
} STACK_ITEM;

// Lista de argumentos
typedef struct entry_list {
  SYMBOL_ENTRY entry;
  struct entry_list *next;
} ENTRY_LIST;

// Nó da AST
typedef struct ast_node {
  char *label;
  LEX_VALUE *valor_lexico;
  int n_children;
  struct ast_node **children;
} AST_NODE;

// Estrutura que representa um lista encadeaada (invertida) de instruções
typedef struct instruction {
  char *code;
  char *arg1;
  char *arg2;
  char *arg3;
  struct instruction *prev;
} INSTRUCTION;

typedef struct prod_value {
  AST_NODE *ast_node;
  SYMBOL_ENTRY table_entry;
  ENTRY_LIST *list;
  ARG_LIST *arg_list;
  int block_return_type;
  // Variáveis para a geração de código ILOC
  char *location;
  INSTRUCTION *code;
} PROD_VALUE;

#endif

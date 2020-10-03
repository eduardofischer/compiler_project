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

typedef union {
  int i;
  float f;
  int b;
  char c;
  char *s;
} TOKEN_VAL;

typedef struct lex_value {
  int line_number;
  int token_type;
  int literal_type;
  TOKEN_VAL value;
} LEX_VALUE;

#endif
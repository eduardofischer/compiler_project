#ifndef TYPES_H
#define TYPES_H

#define LEX_TYPE_SPECIAL 0
#define LEX_TYPE_COMPOUND_OP 1
#define LEX_TYPE_ID 2
#define LEX_TYPE_LIT 3

typedef union {
  int i;
  float f;
  int b;
  char c;
  char *s;
} TOKEN_VAL;

typedef struct lex_value {
  int line_number;
  int type;
  TOKEN_VAL value;
} LEX_VALUE;

#endif